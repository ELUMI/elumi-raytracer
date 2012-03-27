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

void GammaEncode::run(float* buffer, int pixels, int channels) {
  for(int i = 0; i < pixels * channels; i += channels) {
    buffer[i]   = powf(buffer[i],   1.0f / gamma);
    buffer[i+1] = powf(buffer[i+1], 1.0f / gamma);
    buffer[i+2] = powf(buffer[i+2], 1.0f / gamma);
  }
}

}

