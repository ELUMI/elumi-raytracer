/*
 * HilbertCurve.cpp
 *
 *  Created on: Mar 31, 2012
 *      Author: irri
 */

#include "HilbertCurve.h"
#include <iostream>

namespace raytracer {

//convert d to (x,y)
void d2xy(int n, int d, int *x, int *y) {
  int rx, ry, s, t=d;
  *x = *y = 0;
  for (s=1; s<n; s*=2) {
    rx = 1 & (t/2);
    ry = 1 & (t ^ rx);
    rot(s, x, y, rx, ry);
    *x += s * rx;
    *y += s * ry;
    t /= 4;
  }
}

//rotate/flip a quadrant appropriately
void rot(int n, int *x, int *y, int rx, int ry) {
  int t;
  if (ry == 0) {
    if (rx == 1) {
      *x = n-1 - *x;
      *y = n-1 - *y;
    }
    t  = *x;
    *x = *y;
    *y = t;
  }
}

HilbertCurve::HilbertCurve(int width, int height) {
  this->width = width;
  this->height = height;

  n = max(width,height);
  int tmp = n/4;
  batch_size = tmp > 0 ? tmp*tmp : 1;
  nr_of_batches = 16;

  //TODO mindre och mindre allteftersom? dynamiskt?
  batches = new int*[nr_of_batches];

  int x,y;

  for(int i=0; i<nr_of_batches; ++i) {
    batches[i] = new int[batch_size];
    for(int j=0; j<batch_size; ++j) {
      d2xy(n,i*batch_size+j,&x,&y);
      //      x = x%width;
      //      y = y%height;
      int yo = y*width+x;
      batches[i][j] = yo;
      //std::cout << "i=" << i << ",j=" << j << " => "  << i*batch_size+j <<   " (" << x << "," << y << ") => " << yo << "\n";
    }
  }

}

HilbertCurve::~HilbertCurve() {
  for(int i = 0; i < nr_of_batches; ++i)
    delete [] batches[i];
  delete [] batches;
}

int* HilbertCurve::getBatch(int batchNr, int *length) {
  *length = batch_size;
  return batches[batchNr];
}

int HilbertCurve::getNumberBatches() {
  return nr_of_batches;
}

}
