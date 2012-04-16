/*
 * RandomPattern.cpp
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#include "RandomPattern.h"
#include "../utilities/Random.h"

namespace raytracer {

RandomPattern::RandomPattern(int size, int thread_id) : ISuperSamplePattern(size) {
  this->thread_id = thread_id;
}

RandomPattern::~RandomPattern() {
}

void RandomPattern::newPixel(){
  for(int i=0; i<size; ++i){
    offsets[i] = vec2(gen_random_float(thread_id), gen_random_float(thread_id));
  }
}

} /* namespace raytracer */
