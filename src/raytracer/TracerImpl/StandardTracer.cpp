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
  max_recursion_depth = 2;
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
  vec4 refr_color = vec4(0,0,0,0);
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

      // Diffuse lighting
      diffuse += intensity
          * clamp(glm::dot(-light_ray.getDirection(), idata.normal))
      * material->getDiffuse()
      * light->getColor();

      // Specular lighting
      vec3 h = normalize(-incoming_ray.getDirection() - light_ray.getDirection());
      specular += intensity
          * glm::pow( clamp( glm::dot(idata.normal, h) ), material->getShininess() )
      * material->getSpecular()
      * light->getColor();
    }


    // For each light, add ambient component
    ambient += light->getColor();
  }

  // Multiply the accumelated ambient light colors with ambient material color
  ambient *= material->getAmbient();

  if (depth < max_recursion_depth) {

    float refraction_sign = glm::sign(glm::dot(idata.normal, incoming_ray.getDirection()));
    // REFLECTION RAY
    if(material->getReflection() > 0.0f && refraction_sign<0.0f) {
      Ray refl_ray = Ray(idata.interPoint, glm::reflect(incoming_ray.getDirection(),idata.normal));
          //Ray::reflection(incoming_ray, idata.normal, idata.interPoint);
      refl_ray = Ray( refl_ray.getPosition() + idata.normal*0.01f , refl_ray.getDirection() );
      refl_color = tracePrim(refl_ray, depth+1) * material->getReflection() ;//* vec4((vec3(1,1,1)-material->getDiffuse()),1);
    }

    // REFRACTION RAY
    if(material->getOpacity() < 1.0f) {
      Ray refr_ray = Ray::refraction(incoming_ray, idata.normal, idata.interPoint,
          material->getIndexOfRefraction());
      vec3 offset = idata.normal * refraction_sign * 0.01f;
      refr_ray = Ray::generateRay(refr_ray.getPosition() + offset, refr_ray.getDirection());
      refr_color = tracePrim(refr_ray, depth+1) * (1-material->getOpacity());

    }
  }

  // Summarize all color components
  color += (ambient + diffuse + specular) * (1-material->getReflection()) * material->getOpacity();
  vec4 out_color = vec4(color, 1.0f/*material->getOpacity()*/) + refl_color + refr_color;

  return out_color;
}


}
