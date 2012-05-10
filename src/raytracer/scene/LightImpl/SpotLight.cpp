/*
 * PointLight.cpp
 *
 *  Created on: 18 apr 2012
 *      Author: julian
 */

#include "SpotLight.h"
#include "../../utilities/Random.h"
#include <glm/ext.hpp>

namespace raytracer {

SpotLight::~SpotLight() {

}

void SpotLight::getRays(Ray* rays, size_t n, int thread_id) {
  for(size_t i = 0; i<n; ++i){
    rays[i] = Ray(position, gen_random_cone(dir, outer, thread_id));
  }
}

float SpotLight::calcLight(IAccDataStruct* datastruct, vec3 point, int thread_id, vec3 offset) {
  float angle = glm::angle(glm::normalize(point - position), dir);
  if(angle >= outer) // Outside spotlight
    return 0.0f;

  float light = BaseLight::calcLight(datastruct, point, thread_id, offset);

  float falloff_length  = outer - inner;
  if(angle > inner && falloff_length > 0.0f) { // Between inner and outer angle, fall off the light linearly
    float diff = outer - angle;
    return (diff / falloff_length) * light;
  } else {
    return light;
  }
}

} /* namespace raytracer */
