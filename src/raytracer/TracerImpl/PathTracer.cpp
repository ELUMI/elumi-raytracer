/*
 * PathTracer.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: idk
 */

#include "PathTracer.h"
#include "../utilities/Random.h"

namespace raytracer {

PathTracer::PathTracer(Scene* scene) : StandardTracer(scene) {
}

PathTracer::~PathTracer() {
}


vec4 PathTracer::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata,
    float attenuation, unsigned short depth, int thread_id)
{
  if(idata.missed()){
    // No intersection
    return settings->background_color;
  }
  assert(idata.material < scene->getMaterialVector().size());
  // Intersection!
  vec3 normal = idata.normal;
  Material *material = scene->getMaterialVector()[idata.material];
  vec3 color = material->getEmissive();

  float transmittance = (1 - material->getOpacity());
  float reflectance = material->getReflection();

  vec2 tex_coords = getTextureCoordinates(material->getDiffuseMap(),material, idata,vec3(0,0,0));

  if(transmittance < 1.0f && reflectance < 1.0f){
    //color += getAmbient(incoming_ray, idata);
    //color += getLighting(incoming_ray, idata, normal, material, thread_id);

    if(attenuation < ATTENUATION_THRESHOLD || depth > MAX_RECURSION_DEPTH){
    } else {
//      vec3 perturbed_normal = getPerturbedNormal(incoming_ray.getDirection(),
//          idata,material,tex_coords);
      vec3 texture_color = getTextureColor(material, idata,tex_coords);

      Ray new_ray(idata.interPoint, gen_random_hemisphere(normal, thread_id));
      vec3 brdff = brdf(-new_ray.getDirection(), incoming_ray.getDirection(), normal, material, texture_color);
      float new_attenuation = attenuation * (brdff.r+brdff.g+brdff.b)/3;
      vec4 new_color = tracePrim(new_ray, new_attenuation, depth+1, thread_id);
      color += vec3(new_color) * brdff;
    }
  }

  color = reflection_refraction(incoming_ray, idata, attenuation, depth, material, normal, color, tex_coords, thread_id);

  return vec4(color,1.0f);
}


} /* namespace raytracer */
