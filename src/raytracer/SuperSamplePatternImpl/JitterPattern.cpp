/*
 * GridPattern.cpp
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#include "JitterPattern.h"
#include "../utilities/Random.h"

namespace raytracer {

JitterPattern::JitterPattern(int asize, int thread_id) : ISuperSamplePattern(gridify_size(asize)) {
  this->thread_id = thread_id;
}

JitterPattern::~JitterPattern() {
}

void JitterPattern::newPixel() {
  int w = sqrt(size);
  for(int y=0; y<w; ++y){
    for(int x=0; x<w; ++x){
      vec2 p = vec2(x,y);
      p += vec2(gen_random_float(thread_id), gen_random_float(thread_id));
      p/=w;
      offsets[y*w+x] = p;
    }
  }
}

} /* namespace raytracer */
