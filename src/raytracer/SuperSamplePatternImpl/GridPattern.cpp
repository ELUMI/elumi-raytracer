/*
 * GridPattern.cpp
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#include "GridPattern.h"

namespace raytracer {

GridPattern::GridPattern(int asize) : ISuperSamplePattern(gridify_size(asize)) {
  int w = sqrt(size);
  for(int y=0; y<w; ++y){
    for(int x=0; x<w; ++x){
      vec2 p = vec2(x,y);
      p += vec2(0.5f, 0.5f);
      p/=w;
      offsets[y*w+x] = p;
    }
  }
}

GridPattern::~GridPattern() {
}


void GridPattern::newPixel() {
  //done everything needed in constructor
}

} /* namespace raytracer */
