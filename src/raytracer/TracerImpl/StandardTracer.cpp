/*
 * StandardTracer.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */

#include <iostream>

#include "StandardTracer.h"
#include "../utilities/Random.h"
#include "../EnvironmentMapImpl/CubeMap.h"

namespace raytracer {

// TODO report, change threshold comparison images
StandardTracer::StandardTracer(Scene* scene) :
  BaseTracer(scene), MAX_RECURSION_DEPTH(settings->max_recursion_depth),
      ATTENUATION_THRESHOLD(settings->recursion_attenuation_threshold) {
  using_environment_map = false;
}

StandardTracer::~StandardTracer() {

}

void StandardTracer::initTracing() {
  BaseTracer::initTracing();
}

void StandardTracer::first_bounce(){
  BaseTracer::first_bounce();
}

void StandardTracer::traceImage(float* color_buffer) {
  if (scene->getEnvironmentMap() != NULL)
    using_environment_map = true;

  BaseTracer::traceImage(color_buffer);
}

vec4 StandardTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata,
    int thread_id) {
  return shade(ray, idata, 1.0f, 0, thread_id);
}

// Recursive trace method (trace')
vec4 StandardTracer::tracePrim(Ray ray, float attenuation,
    unsigned short depth, int thread_id) {
  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(
      ray);
  return shade(ray, idata, attenuation, depth, thread_id);
}

vec3 StandardTracer::bumpMap(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, Material* material) {

  vec3 normal = idata.normal;

  /**** Bump mapping ****/
  //  if(material->getBumpMap() != -1){
  //    vec3 bump_normal = vec3(0, 0, 0);
  //    Texture *bumpmap = scene->getTextureAt(material->getBumpMap());
  //    vec2 coords = bumpmap->getUVCoordinates(idata.interPoint, idata.e1, idata.e2);
  //    bump_normal = bumpmap->getColorAt(coords);
  //    bump_normal = normalize(faceforward(bump_normal, incoming_ray.getDirection(), normal));
  //    normal = normalize(normal + bump_normal);
  //  }

  vec3 perturbed_normal = vec3(0, 0, 0);

  //Normal mappping

  //fixme

  //  if(material->getNormalMap() != -1) {
  //    Texture* normal_map = scene->getTextureAt(material->getNormalMap());
  //    //vec2 coords  = normal_map->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);
  //
  //    perturbed_normal = normal_map->getColorAt(tex_coords);
  //    perturbed_normal = normalize(faceforward(perturbed_normal,incoming_ray.getDirection(),normal));
  //    normal = normalize(normal+perturbed_normal);
  //  } else if (material->getBumpMap() != -1) { //Bump mapping WIP
  //    Texture* bump_map = scene->getTextureAt(material->getBumpMap());
  //    vec2 c = vec2();
  //    c.x = 1.0f/bump_map->getWidth();
  //    c.y = 1.0f/bump_map->getHeight();
  //
  //    vec2 t1 = tex_coords;
  //    t1.x = t1.x+c.x;
  //    vec2 t2 = tex_coords;
  //    t2.x = t2.x - c.x;
  //    vec2 t3 = tex_coords;
  //    t3.y = t3.y + c.y;
  //    vec2 t4 = tex_coords;
  //    t4.y = t4.y - c.y;
  //
  //    vec3 t0 = vec3(0,0,length(bump_map->getColorAt(tex_coords)));
  //
  //    vec3 v1 = t0 - vec3(0,0,length(bump_map->getColorAt(t1)));
  //    vec3 v2 = t0 - vec3(length(bump_map->getColorAt(t2)));
  //    vec3 v3 = t0 - vec3(length(bump_map->getColorAt(t3)));
  //    vec3 v4 = t0 - vec3(length(bump_map->getColorAt(t4)));
  //
  //    perturbed_normal = v1+v2+v3+v4;
  //    perturbed_normal = normalize(faceforward(perturbed_normal,incoming_ray.getDirection(),normal));
  //
  //    normal = normalize(normal+perturbed_normal);
  //  }

  return normal;
}

vec3 StandardTracer::brdf(vec3 incoming_direction, vec3 outgoing_direction,
    vec3 normal, Material * material, vec3 texture_color) {
  //diffuse
  vec3 color = clamp(glm::dot(-incoming_direction, normal)) * texture_color;

  //specular
  vec3 h = normalize(-outgoing_direction - incoming_direction);
  vec3 specular =
      glm::pow(clamp(glm::dot(normal, h)), material->getShininess())
          * material->getSpecular();

  //fixme
  //  if(material->getSpecularMap() == -1) {
  //    specular *= material->getSpecular();
  //  } else {
  //    Texture *spec = scene->getTextureAt(material->getSpecularMap());
  //    specular *= length(spec->getColorAt(tex_coords));
  //  }

  return color + specular;
}

vec3 StandardTracer::getTextureColor(Material* material,
    IAccDataStruct::IntersectionData idata) {
  if (material->getDiffuseMap() == -1) {
    return material->getDiffuse();
  }

  vec3 texture_color = vec3(0);
  vec2 tex_coords = vec2(0, 0);
  Texture *texture = scene->getTextureAt(material->getDiffuseMap());
  int mipmap_levels = texture->getMipmapLevels();
  if (true) {
    //tex_coords  = texture->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);
    tex_coords = texture->getUVCoordinates(idata.interPoint, YAXIS);
  } else {
    //We have texture coordinates
    tex_coords = idata.texcoord;
  }

  //  if(material->getDiffuseMap() != -1){
  //    Texture *texture = scene->getTextureAt(material->getDiffuseMap());
  //    int mipmap_levels = texture->getMipmapLevels();
  //    if(true){
  //      //tex_coords  = texture->getUVCoordinates(idata.interPoint,idata.e1,idata.e2);
  //      tex_coords = texture->getUVCoordinates(idata.interPoint/4.0f, YAXIS);
  //    }else{
  //      //We have texture coordinates
  //      tex_coords = idata.texcoord;
  //    }
  //    if(false){
  //      float d = 0;
  //      if(d > 0){
  //        Texture *mmp_bottom = scene->getTextureAt(material->getDiffuseMap() + (int)(d));
  //        Texture *mmp_top = scene->getTextureAt(material->getDiffuseMap() + (int)(d) + 1);
  //        texture_color = ((1.0f - d - floor(d)) * mmp_bottom->getColorAt(tex_coords) + d - floor(d) * mmp_top->getInterpolatedColor(tex_coords));
  //      }else{
  //        texture_color = texture->getColorAt(tex_coords);
  //      }
  //      //float dv = 1.0f/(float)mipmap_levels;
  //    }
  //    else{
  //      texture_color = texture->getColorAt(tex_coords);
  //    }
  //  }

  //Parallax mapping
  if (material->getBumpMap() != -1 && material->getDiffuseMap() != -1) {
    Texture* bump_map = scene->getTextureAt(material->getBumpMap());
    float height = glm::length(bump_map->getColorAt(tex_coords));
    vec3 p = -height * scene->getCamera().getPosition() - idata.interPoint;
    p = normalize(p);
    p = p * 0.2f;

    tex_coords.x += p.x;
    tex_coords.y += p.z;

    Texture* t = scene->getTextureAt(material->getDiffuseMap());

    texture_color = t->getColorAt(tex_coords);
  }

  if (false) {
    float d = 0;
    if (d > 0) {
      Texture *mmp_bottom = scene->getTextureAt(
          material->getDiffuseMap() + (int) (((((d))))));
      Texture *mmp_top = scene->getTextureAt(
          material->getDiffuseMap() + (int) (((((d))))) + 1);
      return ((1.0f - d - floor(d)) * mmp_bottom->getColorAt(tex_coords) + d
          - floor(d) * mmp_top->getInterpolatedColor(tex_coords));
    } else {
      return texture->getColorAt(tex_coords);
    }
    //float dv = 1.0f/(float)mipmap_levels;
  } else {
    return texture->getColorAt(tex_coords);
  }
  return texture_color;
}

inline vec3 StandardTracer::reflection_refraction(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, float attenuation,
    unsigned short depth, Material *material, vec3 normal, vec3 color,
    int thread_id) {

  if (attenuation < ATTENUATION_THRESHOLD || depth > MAX_RECURSION_DEPTH) {
    return color;
  }

  const float offset_size = 0.01f;
  float reflectance = material->getReflection();
  float transmittance = (1 - material->getOpacity());

  //Transparency map
  //fixme
  //  if(material->getTransparencyMap() != -1) {
  //    Texture* transparencymap = scene->getTextureAt(material->getTransparencyMap());
  //    transmittance = glm::length(transparencymap->getColorAt(tex_coords));
  //  }

  // REFRACTION_SIGN: //missledande namn, tecknet är vilket håll offsetten typ förskjuts
  // Ray enters mesh => -1
  // Ray leaves mesh => 1
  const float refraction_sign = glm::sign(
      glm::dot(normal, incoming_ray.getDirection()));
  const vec3 refr_normal = -normal * refraction_sign;

  //Fresnel reflectance
  if (material->getFresnelIndex() != 0.0f) {
  //if (settings->use_fresnel) {
    //Schlick's approx.
//    float fresnel_refl = reflectance + (1 - reflectance) * glm::pow(
//    clamp(1.0f - abs(glm::dot(-incoming_ray.getDirection(), normal))), 5.0f);
//
//    cout << fresnel_refl << "\n";


    float c = abs(glm::dot(incoming_ray.getDirection(), normal));
    float g = glm::sqrt( material->getFresnelIndex()*material->getFresnelIndex() + c*c -1 );
    float gMc = g-c;
    float gPc = g+c;
    float fresnel_refl = 0.5 * ((gMc*gMc) / (gPc*gPc)) *
                         (  1.0f +
                             ( (c*(gPc)-1)*(c*(gPc)-1) )
                           / ( (c*(gMc)+1)*(c*(gMc)+1) )
                         );
    reflectance = fresnel_refl;
  }

  /**** REFRACTION RAY ****/
  if (transmittance > 0.0f /*&& reflectance < 1.0f*/) {

    float refract_spread = material->getRefractionSpread();
    int refract_samples = material->getRefractionSamples();
    float eta = material->getIndexOfRefraction();

    if (refraction_sign == -1.0f)
      eta = 1 / eta;

    vec3 offset = -refr_normal * offset_size;
    vec3 refr_dir = glm::refract(incoming_ray.getDirection(), refr_normal, eta);

    if (refr_dir == vec3(0, 0, 0)) {
      // Refraction direction imaginary => Total internal reflection
      refr_dir = glm::reflect(incoming_ray.getDirection(), refr_normal);
    }

    Ray refr_ray = Ray(idata.interPoint + offset, refr_dir);
    vec4 refr_color;

    if (refract_spread > 0.0f && refract_samples > 0) { // Glossy refractions
      for (int i = 0; i < refract_samples; ++i) {

        vec3 sample_dir = glm::normalize(
            gen_random_cone(refr_dir, refract_spread, thread_id));

        Ray sample_ray = Ray(idata.interPoint + offset, sample_dir);

        refr_color += tracePrim(sample_ray,
            attenuation * (transmittance) / refract_samples, depth + 1,
            thread_id) * (transmittance) / refract_samples;

      }
    } else { // Non glossy
      refr_color = tracePrim(refr_ray, attenuation * (transmittance),
          depth + 1, thread_id) * (transmittance);
    }
    // SVART BEROR PÅ SELF SHADOWING
    //mix with output
    color = color * (1 - transmittance) + vec3(refr_color);
  } // end refraction

  /**** REFLECTION RAY ****/
  if (reflectance > 0.0f) {
    vec3 offset = refr_normal * offset_size;
    vec3 refl_dir = glm::reflect(incoming_ray.getDirection(), refr_normal);
    Ray refl_ray = Ray(idata.interPoint + offset, glm::normalize(refl_dir));

    vec4 refl_color;

    float reflect_spread = material->getReflectionSpread();
    int reflect_samples = material->getReflectionSamples();

    if (reflect_spread > 0.0f && reflect_samples > 0) { // Glossy reflections
      for (int i = 0; i < reflect_samples; ++i) {

        vec3 sample_dir = glm::normalize(
            gen_random_cone(refl_dir, reflect_spread, thread_id));

        Ray sample_ray = Ray(idata.interPoint + offset, sample_dir);

        refl_color += tracePrim(sample_ray,
            attenuation * reflectance / reflect_samples, depth + 1, thread_id)
            * reflectance / reflect_samples;

      }
    } else { // Non glossy
      refl_color = tracePrim(refl_ray, attenuation * reflectance, depth + 1,
          thread_id) * reflectance;
    }
    // SVART BEROR PÅ ATT DEN STUDSAR MOT SIG SJÄLV OCH ALLTID BLIR REFLECTIVE TILLS MAXDJUP
    //mix with output
    color = color * (1 - reflectance) + vec3(refl_color);
  }



  return color;
}

vec3 StandardTracer::getAmbient(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, int thread_id, unsigned short depth) {
  Material *material = scene->getMaterialVector()[idata.material];
  vec3 color = vec3(0);
  for (unsigned int i = 0; i < lights->size(); ++i) {
    ILight *light = lights->at(i);
    if (light->getFalloffType() == ILight::NONE) {
      //do nothing, handled in getAmbient()
      color += light->getColor() * material->getAmbient();
    }
  }

  return color;
}

vec3 StandardTracer::getLighting(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, vec3 normal, Material *material,
    int thread_id) {
  vec3 texture_color = getTextureColor(material, idata);
  vec3 color = vec3(0);

  /**** For each light source in the scene ****/
  //if (material->getReflection() < 1.0f || material->getTransparencyMap() > 0.0f)
    for (unsigned int i = 0; i < lights->size(); ++i) {
      ILight *light = lights->at(i);
      if (light->getFalloffType() == ILight::NONE) {
        //do nothing, handled in getAmbient()
      } else {
        /**** NON AMBIENT LIGHT, CALCULATE SHADOW RAYS ***/
        float in_light = light->calcLight(datastruct, idata.interPoint,
            thread_id);
        if (in_light > 0.0f) {
          // NOT ENTIRELY IN SHADOW! SHADE!
          Ray light_ray = Ray::generateRay(light->getPosition(),
              idata.interPoint);
          color += light->getColor() * in_light * brdf(
              light_ray.getDirection(), incoming_ray.getDirection(), normal,
              material, texture_color);
        }
      }
    }

  return color;
}

vec4 StandardTracer::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, float attenuation,
    unsigned short depth, int thread_id) {
  if (idata.missed()) {
    // No intersection

    //    vec3 light_color = vec3(0,0,0);
    //
    //    for(unsigned int i=0; i<lights->size(); ++i) {
    //      ILight* light  = lights->at(i);
    //      light_color += light->getColor();
    //      Ray light_ray = Ray::generateRay(light->getPosition(), scene->getCamera().getPosition());
    //      vec3 l = normalize(light_ray.getDirection());
    //      vec3 r = normalize(incoming_ray.getDirection());
    //      float li = dot(l,r);
    //      light_color *= li*li;
    //      //vec3 lc = light->getPosition()-scene->getCamera().getPosition();
    //      //light_color *= light->getIntensity(lc.length());
    //    }
    //files
    //    return settings->background_color + vec4(light_color.r,light_color.g,light_color.b,1.0f);

    if (using_environment_map)
      return vec4(scene->getEnvironmentMap()->getColor(incoming_ray), 1.0f);
    return settings->background_color;
  }
  assert(idata.material < scene->getMaterialVector().size());
  // Intersection!
  Material *material = scene->getMaterialVector()[idata.material];
  vec3 normal = bumpMap(incoming_ray, idata, material);
  vec3 color = material->getEmissive();
  color += getAmbient(incoming_ray, idata, thread_id, depth);
  color += getLighting(incoming_ray, idata, normal, material, thread_id);
  color = reflection_refraction(incoming_ray, idata, attenuation, depth,
      material, normal, color, thread_id);

  return vec4(color, 1.0f);
}

}
