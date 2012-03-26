/*
 * LinePattern.cpp
 *
 *  Created on: 19 mar 2012
 *      Author: julian
 */


#include "LinePattern.h"

namespace raytracer {

LinePattern::LinePattern(int width, int height) {
  this->width = width;
  this->height = height;

  batches = new int*[height];

  for(int i = 0; i < height; ++i) {
    batches[i] = new int[width];
    for(int j = 0; j < width; ++j)
      batches[i][j] = i * width + j;
  }
}

LinePattern::~LinePattern() {
  for(int i = 0; i < height; ++i)
    delete [] batches[i];
  delete [] batches;
}

int LinePattern::getNumberBatches() {
  return height;
}

int* LinePattern::getBatch(int batchNr, int* length) {
  *length = width;
  return batches[batchNr];
}



}

