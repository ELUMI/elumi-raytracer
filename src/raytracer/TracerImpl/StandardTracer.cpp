/*
 * StandardTracer.cpp
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */


#include "StandardTracer.h"

namespace raytracer {

StandardTracer::StandardTracer(Scene* scene, Settings* settings)
: BaseTracer(scene, settings) {

}

StandardTracer::~StandardTracer() {

}

void StandardTracer::traceImage(float* color_buffer) {
  max_recursion_depth = 3;
  BaseTracer::traceImage(color_buffer);
}

vec4 StandardTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata) {
  return shade(ray, idata, 0);
}

vec4 StandardTracer::tracePrim(Ray ray,
    const unsigned int depth) {
  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  return shade(ray, idata, depth);
}

vec4 StandardTracer::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata,
    const unsigned int depth) {
  if (idata.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    // No intersection
    return settings->background_color;
  }
  assert(idata.material < scene->getMaterialVector().size());

  // Intersection!

  vec3 color      = vec3(0,0,0);
  vec3 ambient    = vec3(0,0,0);
  vec3 diffuse    = vec3(0,0,0);
  vec3 specular   = vec3(0,0,0);
  vec4 refl_color = vec4(0,0,0,0);
  vec3 refr_color = vec4(0,0,0);
  Material* material = scene->getMaterialVector()[idata.material];

  // For each light source in the scene
  for(unsigned int i=0; i<lights->size(); ++i) {
    ILight* light  = lights->at(i);
    Ray light_ray = Ray::generateRay(light->getPosition(), idata.interPoint);
    IAccDataStruct::IntersectionData light_idata = datastruct->findClosestIntersection(light_ray);

    float distance_to_light = length(idata.interPoint - light->getPosition()); // 1
    float distance_between_light_and_first_hit = length(light_idata.interPoint - light->getPosition()); // 2

    if (light_idata.material != IAccDataStruct::IntersectionData::NOT_FOUND
        && (distance_between_light_and_first_hit + 0.0001f) < distance_to_light) {
      // IN SHADOW!
      //diffuse = vec3(1,0,0);
    } else {

      // Falloff intensity
      float intensity = light->getIntensity(distance_to_light);

      Ray inv_light_ray = Ray(idata.interPoint, -light_ray.getDirection());

      // Diffuse lighting
      diffuse += intensity
          * clamp(glm::dot(inv_light_ray.getDirection(), idata.normal))
      * material->getDiffuse()
      * light->getColor();

      // Specular lighting
      vec3 h = normalize(inv_light_ray.getDirection() + incoming_ray.getDirection());
      specular += intensity
          * glm::pow( clamp( glm::dot(idata.normal, h) ), material->getShininess() )
      * material->getSpecular()
      * light->getColor();
    }
    //    else
    //      diffuse = vec3(1,0,0);

    // For each light, add ambient component
    ambient += light->getColor();
  }

  // Multiply the accumelated ambient light colors with ambient material color
  ambient *= material->getAmbient();

  if (depth < max_recursion_depth) {

    // REFLECTION RAY
//    if(material->getReflection() > 0.0f) {
//      cout << material->getReflection();
//      Ray refl_ray = Ray::reflection(incoming_ray, idata.normal, idata.interPoint);
//      refl_color = tracePrim(refl_ray, depth+1) * material->getReflection() ;//* vec4((vec3(1,1,1)-material->getDiffuse()),1);
//    }

    // REFRACTION RAY
    if(material->getOpacity() < 1.0f) {
      Ray refr_ray = Ray::refraction(incoming_ray, idata.normal, idata.interPoint,
          material->getIndexOfRefraction());
      vec3 newpos = refr_ray.getPosition() + refr_ray.getDirection()*0.01f;
      refr_ray = Ray::generateRay(newpos, refr_ray.getDirection());
      refr_color = tracePrim(refr_ray, depth+1) material->getOpacity();
    }
  }

  // Summarize all color components
  color += (ambient + diffuse + specular);
  return vec4( color, material->getOpacity()) + refl_color + refr_color;
}


}
