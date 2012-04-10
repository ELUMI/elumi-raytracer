/*
 * ISuperSamplePattern.cpp
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#include "ISuperSamplePattern.h"

namespace raytracer {

ISuperSamplePattern::ISuperSamplePattern(int size) {
  this->size = size;
  offsets = new vec2[size];
}

ISuperSamplePattern::~ISuperSamplePattern() {
  delete [] offsets;
}

const vec2* ISuperSamplePattern::getOffsets() const {
  return offsets;
}

const vec2* ISuperSamplePattern::getNewOffsets(){
  newPixel();
  return getOffsets();
}

const int ISuperSamplePattern::getSize() const {
  return size;
}

int gridify_size(int i){
  i = floor(sqrt(i));
  return i*i;
}


} /* namespace raytracer */
