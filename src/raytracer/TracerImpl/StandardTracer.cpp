/*
 * StandardTracer.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */




#include "StandardTracer.h"

namespace raytracer {

// TODO rapport ändra threshold jämförelsebilder
StandardTracer::StandardTracer(Scene* scene, Settings* settings)
: BaseTracer(scene, settings)
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
    return settings->background_color;
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

  vec3 bump_normal = vec3(0,0,0);
  vec3 normal = idata.normal;
  vec2 tex_coords = vec2(0,0);

  if(material->getTexture() != -1) {

    Texture* texture = scene->getTextureAt(material->getTexture());

    //Planar mapping
    if(idata.uvcoords.size() ==  0) {
      tex_coords  = texture->getUVCoordinates(idata.interPoint,PLANAR);
      texture_color = texture->getColorAt(tex_coords);
    } else { //We have texture coordinates
      texture_color = texture->getColorAt(idata.texcoord);
      tex_coords = idata.texcoord;
    }
  }

  /**** Bump mapping ****/
  if(material->getBumpMap() != -1) {
    Texture* bumpmap = scene->getTextureAt(material->getBumpMap());
    bump_normal = bumpmap->getColorAt(tex_coords);
    bump_normal = normalize(faceforward(bump_normal,incoming_ray.getDirection(),normal));

    normal = normalize(normal+bump_normal);
  }

  /**** For each light source in the scene ****/
  for(unsigned int i=0; i<scene->getLightVector().size(); ++i) {
    ILight* light  = scene->getLightVector().at(i);

    if (light->getFalloffType() == ILight::NONE) {
      ambient += light->getColor();
    } else {
      /**** NON AMBIENT LIGHT, CALCULATE SHADOW RAYS ***/

      Ray light_ray = Ray::generateRay(light->getPosition(), idata.interPoint);
      float distance_to_light = length(idata.interPoint - light->getPosition());

      /**** IF IN SHADOW ****/
      //     if (light_idata.material != IAccDataStruct::IntersectionData::NOT_FOUND
      //         && (distance_between_light_and_first_hit + 0.0001f) < distance_to_light) {
      if(light->isBlocked(datastruct, idata.interPoint)) {

        //diffuse = vec3(1,0,0);
      } else {

        // Falloff intensity
        float intensity = light->getIntensity(distance_to_light);

        //Diffuse
        diffuse += intensity
            * clamp(glm::dot(-light_ray.getDirection(), normal))
        * light->getColor();

        if(material->getTexture() == -1) {
          diffuse *= material->getDiffuse();
        } else {
          diffuse *= texture_color;
        }

        //Specular
        vec3 h = normalize(-incoming_ray.getDirection() - light_ray.getDirection());
        specular += intensity
            * glm::pow( clamp( glm::dot(normal, h) ), material->getShininess() )
        * material->getSpecular()
        * light->getColor();

      } // end diffuse specular
    } // end non abmient
  } // for each light

  // Multiply the accumelated ambient light colors with ambient material color
  ambient *= material->getAmbient();

  float reflectance = material->getReflection();
  float transmittance = (1-material->getOpacity());

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
  vec4 color = vec4((ambient + diffuse + specular),1.0f) * (1-transmittance) + refr_color;
  color *= (1-reflectance);
  color += refl_color;
  color.a = 1.0f;

  return color;
}

}
