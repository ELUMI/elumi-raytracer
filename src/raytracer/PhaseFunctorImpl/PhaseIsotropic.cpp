/*
 * PhaseIsotropic.cpp
 *
 *  Created on: 17 apr 2012
 *      Author: julian
 */

#include "PhaseIsotropic.h"
#include "../utilities/glutil.h"

namespace raytracer {


float PhaseIsotropic::p(vec3 &, vec3 &) {
  return 1.0f / (4.0f * M_PI);
}

} /* namespace raytracer */
