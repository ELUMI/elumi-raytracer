/*
 * ClampOperator.cpp
 *
 *  Created on: 26 feb 2012
 *      Author: julian
 */

#include "ClampOperator.h"

namespace raytracer {

ClampOperator::ClampOperator() {
}

void ClampOperator::run(float* buffer, int pixels, int channels) {
  for(int i = 0; i < pixels * channels; i += channels) {
    buffer[i]   = buffer[i]   > 1.0f ? 1.0f : buffer[i];
    buffer[i+1] = buffer[i+1] > 1.0f ? 1.0f : buffer[i+1];
    buffer[i+2] = buffer[i+2] > 1.0f ? 1.0f : buffer[i+2];
  }
}

}
