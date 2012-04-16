/*
 * UniformVolume.cpp
 *
 *  Created on: 10 apr 2012
 *      Author: julian
 */

#include "UniformVolume.h"

namespace raytracer {

UniformVolume::UniformVolume(OBB obb, float sigma_t, float emission) {
  UniformVolume::obb = obb;
  UniformVolume::sigma_t = sigma_t;
  UniformVolume::emission = emission;
}

UniformVolume::~UniformVolume() {
}

float UniformVolume::getTau(vec3 from, vec3 to) {
  float d = glm::distance(from, to);
  return sigma_t * d;
}

OBB::IntervalData UniformVolume::getInterval(Ray ray) {
  return obb.rayInterval(ray);
}

float UniformVolume::getEmission(vec3 pos, vec3 dir) {

}

} /* namespace raytracer */
