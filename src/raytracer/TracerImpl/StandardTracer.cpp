/*
 * StandardTracer.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */




#include "StandardTracer.h"

namespace raytracer {

// TODO rapport ändra threshold jämförelsebilder
StandardTracer::StandardTracer(Scene* scene)
: BaseTracer(scene)
, MAX_RECURSION_DEPTH(settings->max_recursion_depth)
, ATTENUATION_THRESHOLD(settings->recursion_attenuation_threshold) {

}

StandardTracer::~StandardTracer() {

}


void StandardTracer::traceImage(float* color_buffer) {
  BaseTracer::traceImage(color_buffer);
}

vec4 StandardTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata) {
  return shade(ray, idata, 1.0f, 0);
}

vec4 StandardTracer::tracePrim(Ray ray, float attenuation, unsigned short depth) {
  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  return shade(ray, idata, attenuation, depth);
}

vec4 StandardTracer::shade(Ray incoming_ray
    , IAccDataStruct::IntersectionData idata
    , float attenuation
    , unsigned short depth) {

  if (idata.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    // No intersection

    vec3 light_color = vec3(0,0,0);

    for(unsigned int i=0; i<lights->size(); ++i) {
      ILight* light  = lights->at(i);
      light_color += light->getColor();
      Ray light_ray = Ray::generateRay(light->getPosition(), scene->getCamera().getPosition());
      vec3 l = normalize(light_ray.getDirection());
      vec3 r = normalize(incoming_ray.getDirection());
      float li = dot(l,r);
      light_color *= li*li;
      //vec3 lc = light->getPosition()-scene->getCamera().getPosition();
      //light_color *= light->getIntensity(lc.length());
    }

    return settings->background_color + vec4(light_color.r,light_color.g,light_color.b,1.0f);
  }
  assert(idata.material < scene->getMaterialVector().size());

  // Intersection!

  vec3 ambient    = vec3(0,0,0);
  vec3 diffuse    = vec3(0,0,0);
  vec3 specular   = vec3(0,0,0);
  vec4 refl_color = vec4(0,0,0,0);
  vec4 refr_color = vec4(0,0,0,0);
  Material* material = scene->getMaterialVector()[idata.material];

  vec3 texture_color = vec3(0,0,0);
  vec3 specularity = vec3(0,0,0);

  vec3 perturbed_normal = vec3(0,0,0);
  vec3 normal = idata.normal;
  vec2 tex_coords = vec2(0,0);
  vec3 light_color = vec3(0,0,0);

  if(material->getDiffuseMap() != -1) {

    Texture* texture = scene->getTextureAt(material->getDiffuseMap());
    int mipmap_levels = texture->getMipmapLevels();

    if(true) {
      //tex_coords  = texture->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);
      tex_coords = texture->getUVCoordinates(idata.interPoint/4.0f,YAXIS);
    } else { //We have texture coordinates
      tex_coords = idata.texcoord;
    }

    if(false) {

      float d = 0;

      if(d > 0) {
        Texture* mmp_bottom = scene->getTextureAt(material->getDiffuseMap()+(int)d);
        Texture* mmp_top = scene->getTextureAt(material->getDiffuseMap()+(int)d+1);

        texture_color = ( (1.0f-d-floor(d))*mmp_bottom->getColorAt(tex_coords) + d-floor(d)*mmp_top->getInterpolatedColor(tex_coords) );
      } else {
        texture_color = texture->getColorAt(tex_coords);
      }

      //float dv = 1.0f/(float)mipmap_levels;

    } else {
      texture_color = texture->getColorAt(tex_coords);
    }

  }

  //Normal mappping
  if(material->getNormalMap() != -1) {
    Texture* normal_map = scene->getTextureAt(material->getNormalMap());
    //vec2 coords  = normal_map->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);

    //Parallax mapping
    if(material->getBumpMap() != -1 && material->getDiffuseMap() != -1) {
      Texture* bump_map = scene->getTextureAt(material->getBumpMap());
      float height = glm::length(bump_map->getColorAt(tex_coords));
      vec3 p = -height*scene->getCamera().getPosition()-idata.interPoint;
      p = normalize(p);
      p = p*0.2f;

      tex_coords.x += p.x;
      tex_coords.y += p.z;

      Texture* t = scene->getTextureAt(material->getDiffuseMap());

      texture_color = t->getColorAt(tex_coords);
    }

    perturbed_normal = normal_map->getColorAt(tex_coords);
    perturbed_normal = normalize(faceforward(perturbed_normal,incoming_ray.getDirection(),normal));
    normal = normalize(normal+perturbed_normal);
  } else if (material->getBumpMap() != -1) { //Bump mapping WIP
    Texture* bump_map = scene->getTextureAt(material->getBumpMap());
    vec2 c = vec2();
    c.x = 1.0f/bump_map->getWidth();
    c.y = 1.0f/bump_map->getHeight();

    vec2 t1 = tex_coords;
    t1.x = t1.x+c.x;
    vec2 t2 = tex_coords;
    t2.x = t2.x - c.x;
    vec2 t3 = tex_coords;
    t3.y = t3.y + c.y;
    vec2 t4 = tex_coords;
    t4.y = t4.y - c.y;

    vec3 t0 = bump_map->getColorAt(tex_coords);

    vec3 v1 = t0 - bump_map->getColorAt(t1);
    vec3 v2 = t0 - bump_map->getColorAt(t2);
    vec3 v3 = t0 - bump_map->getColorAt(t3);
    vec3 v4 = t0 - bump_map->getColorAt(t4);

    perturbed_normal = v1+v2+v3+v4;
    perturbed_normal = normalize(faceforward(perturbed_normal,incoming_ray.getDirection(),normal));

    normal = normalize(normal+perturbed_normal);
  }

  /**** For each light source in the scene ****/
  for(unsigned int i=0; i<lights->size(); ++i) {
    ILight* light  = lights->at(i);

    if (light->getFalloffType() == ILight::NONE) {
      ambient += light->getColor();
    } else {
      /**** NON AMBIENT LIGHT, CALCULATE SHADOW RAYS ***/

      //     if (light_idata.material != IAccDataStruct::IntersectionData::NOT_FOUND
      //         && (distance_between_light_and_first_hit + 0.0001f) < distance_to_light) {
      //if(light->isBlocked(datastruct, idata.interPoint)) {
      float in_light = light->calcLight(datastruct, idata.interPoint);
      if (in_light > 0.0f) {
        // NOT ENTIRELY IN SHADOW! SHADE!
        Ray light_ray = Ray::generateRay(light->getPosition(), idata.interPoint);
//        float distance_to_light = length(idata.interPoint - light->getPosition());
//
//        // Falloff intensity
//        float intensity = light->getIntensity(distance_to_light);
//in_light = intensity;


        //Diffuse
        diffuse += in_light
            * clamp(glm::dot(-light_ray.getDirection(), normal))
        * light->getColor();

        if(material->getDiffuseMap() == -1) {
          diffuse *= material->getDiffuse();
        } else {
          diffuse *= texture_color;
        }

        //Specular
        vec3 h = normalize(-incoming_ray.getDirection() - light_ray.getDirection());
        specular += in_light
            * glm::pow( clamp( glm::dot(normal, h) ), material->getShininess() )
        * light->getColor();

        if(material->getSpecularMap() == -1) {
          specular *= material->getSpecular();
        } else {
          specular *= specularity;
        }

      } // end in light
    } // end non abmient
  } // for each light

  // Multiply the accumelated ambient light colors with ambient material color
  ambient *= material->getAmbient();

  float reflectance = material->getReflection();

  float transmittance = (1-material->getOpacity());

  //Transparency map
  if(material->getTransparencyMap() != -1) {
    Texture* transparencymap = scene->getTextureAt(material->getTransparencyMap());
    transmittance = glm::length(transparencymap->getColorAt(tex_coords));

  }

  if (attenuation > ATTENUATION_THRESHOLD && depth < MAX_RECURSION_DEPTH) {

    // REFRACTION_SIGN:
    // Ray enters mesh => -1
    // Ray leaves mesh => 1
    float refraction_sign = glm::sign(glm::dot(normal, incoming_ray.getDirection()));
    vec3 refr_normal = -normal * refraction_sign;

    /**** REFLECTION RAY ****/
    if(reflectance > 0.0f) {

      // Fresnel reflectance (with Schlick's approx.)
      //      float fresnel_refl = reflectance
      //          + (1 - reflectance)
      //          * glm::pow( clamp(1.0f + glm::dot(incoming_ray.getDirection(), normal) ), 5.0f);
      //      reflectance = fresnel_refl;


      vec3 offset = refr_normal * 0.01f;
      vec3 refl_dir = glm::reflect(incoming_ray.getDirection(), refr_normal);
      Ray refl_ray = Ray(idata.interPoint + offset, glm::normalize(refl_dir));
      refl_color = tracePrim(refl_ray, attenuation*reflectance, depth+1) * reflectance;

      // SVART BEROR PÅ ATT DEN STUDSAR MOT SIG SJÄLV OCH ALLTID BLIR REFLECTIVE TILLS MAXDJUP

    }

    /**** REFRACTION RAY ****/
    if(transmittance > 0.0f && reflectance < 1.0f) {

      float eta = material->getIndexOfRefraction();
      if(refraction_sign == -1.0f)
        eta = 1/eta;

      vec3 offset = -refr_normal * 0.01f;
      vec3 refr_dir = glm::refract(incoming_ray.getDirection(), refr_normal, eta);
      if (refr_dir == vec3(0,0,0)) {
        transmittance = 0.0f;
      } else {

        Ray refr_ray = Ray(idata.interPoint + offset, refr_dir);
        refr_color = tracePrim(refr_ray, attenuation*(transmittance), depth+1) * (transmittance);

        // SVART BEROR PÅ SELF SHADOWING
      }

    } // end refraction
  } // end annutation

  // Mix the output colors
  vec4 color = vec4((ambient + diffuse + specular + light_color),1.0f) * (1-transmittance) + refr_color;
  color *= (1-reflectance);
  color += refl_color;
  color.a = 1.0f;

  return color;
}

}
