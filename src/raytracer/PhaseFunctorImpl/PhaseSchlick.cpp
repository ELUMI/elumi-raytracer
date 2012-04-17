/*
 * PhaseSchlick.cpp
 *
 *  Created on: 17 apr 2012
 *      Author: julian
 */

#include "PhaseSchlick.h"
#include "../utilities/glutil.h"

#include "glm/glm.hpp"

namespace raytracer {

using namespace glm;

PhaseSchlick::PhaseSchlick(float k) {
  PhaseSchlick::k = k;
}

float PhaseSchlick::p(vec3& w_in, vec3& w_out) {
  return 1.0f / (4.0f * M_PI) *
      (1.0f - k*k) / pow(1.0f - k * dot(w_in, w_out), 2);
}

} /* namespace raytracer */
