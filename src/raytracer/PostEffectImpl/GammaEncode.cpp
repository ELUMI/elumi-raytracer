/*
 * GammaEncode.cpp
 *
 *  Created on: 26 feb 2012
 *      Author: julian
 */

#include "GammaEncode.h"
#include <glm/glm.hpp>

namespace raytracer {

GammaEncode::GammaEncode(float gamma) {
  GammaEncode::gamma = gamma;
}

float* GammaEncode::run(float* buffer, int length) {
  for(int i = 0; i < length; i++) {
    buffer[i] = powf(buffer[i], 1.0f / gamma);
  }

  return buffer;
}

}

