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

float* ClampOperator::run(float* buffer, int length) {
  for(int i = 0; i < length; i++) {
    buffer[i] = buffer[i] > 1.0f ? 1.0f : buffer[i];
  }

  return buffer;
}

}
