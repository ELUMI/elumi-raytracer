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

unsigned int nextOrEqualPowerOf2 (unsigned int x) {
  unsigned int powerOfTwo = 1;

  while (powerOfTwo < x && powerOfTwo < 2147483648)
    powerOfTwo *= 2;
  return powerOfTwo;
}

HilbertCurve::HilbertCurve(int width, int height) {
  this->width = width;
  this->height = height;

  unsigned int max_side = max(width,height);
  n = nextOrEqualPowerOf2(max_side);

  nr_of_batches = 32; // Must be power of 2
  unsigned int max_batch_size = (n*n)>=nr_of_batches ? (n*n)/nr_of_batches : 1;
  //max_batch_size = max_batch_size*max_batch_size;


  //TODO mindre och mindre allteftersom? dynamiskt?
  batches = new int*[nr_of_batches];
  batch_sizes = new int[nr_of_batches];
  int* batch_errors = new int[nr_of_batches];

  int x,y;

  // Create hilbert pattern (may be larger than image)
  for(unsigned int i=0; i<nr_of_batches; ++i) {
    batches[i] = new int[max_batch_size];
    batch_errors[i] = 0;

    for(unsigned int j=0; j<max_batch_size; ++j) {
      d2xy(n,i*max_batch_size+j,&x,&y);
      int yo = y*width+x;
      if (x>=width || y>=height) {
        batches[i][j] = -1;
        batch_errors[i]++;
      } else {

        batches[i][j] = yo;
      }
      //std::cout << "i=" << i << ",j=" << j << " => "  << i*max_batch_size+j <<   " (" << x << "," << y << ") => " << yo << "\n";
    }

  }

  // Remove unnessesary coordinates
  for(unsigned int i=0; i<nr_of_batches; ++i) {
    int apa = batch_errors[i];
    int this_batch_size = max_batch_size-batch_errors[i];
    batch_sizes[i] = this_batch_size;
    int* batch = new int[this_batch_size];

    int k = 0;
    for(unsigned int j=0; k<this_batch_size; ++j) {
      int val = batches[i][j];
      if (val != -1) {
        batch[k++] = val;
      }
    }

    delete batches[i];
    batches[i] = batch;

  }

  delete [] batch_errors;

}

HilbertCurve::~HilbertCurve() {
  for(int i = 0; i < nr_of_batches; ++i)
    delete [] batches[i];
  delete [] batches;

  delete [] batch_sizes;
}

int* HilbertCurve::getBatch(int batchNr, int *length) {
  *length = batch_sizes[batchNr];
  return batches[batchNr];
}

int HilbertCurve::getNumberBatches() {
  return nr_of_batches;
}

}
