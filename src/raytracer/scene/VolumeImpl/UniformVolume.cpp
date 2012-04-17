/*
 * UniformVolume.cpp
 *
 *  Created on: 10 apr 2012
 *      Author: julian
 */

#include "UniformVolume.h"
#include "../../IPhaseFunctor.h"

namespace raytracer {

UniformVolume::UniformVolume(OBB obb, float absorption, float scattering, float emission, IPhaseFunctor* pf) {
  UniformVolume::obb = obb;
  UniformVolume::absorption = absorption;
  UniformVolume::scattering = scattering;
  UniformVolume::emission = emission;
  UniformVolume::pf = pf;
}

UniformVolume::~UniformVolume() {
}

float UniformVolume::getTau(vec3 from, vec3 to) {
  float d = glm::distance(from, to);
  return (absorption + scattering) * d;
}

OBB::IntervalData UniformVolume::getInterval(Ray ray) {
  return obb.rayInterval(ray);
}

float UniformVolume::getEmission(vec3 pos, vec3 dir) {
  return emission;
}

float UniformVolume::getPhase(vec3& w_in, vec3& w_out) {
  return pf->p(w_in, w_out);
}

float UniformVolume::getScattering() {
  return scattering;
}

} /* namespace raytracer */
