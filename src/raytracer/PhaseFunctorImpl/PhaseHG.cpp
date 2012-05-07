/*
 * PhaseHG.cpp
 *
 *  Created on: 17 apr 2012
 *      Author: julian
 */

#include "PhaseHG.h"
#include "../GLData/glutil.h"

namespace raytracer {

using namespace glm;

PhaseHG::PhaseHG(float g) {
  PhaseHG::g = g;
}

float PhaseHG::p(vec3& w_in, vec3& w_out) {
  return 1.0f / (4.0f * M_PI) *
      (1.0f - g*g) / pow(1.0f + g*g - 2*g * dot(w_in, w_out), 1.5f);
}

} /* namespace raytracer */
