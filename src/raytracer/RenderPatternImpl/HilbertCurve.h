/*
 * HilbertCurve.h
 *
 *  Created on: Mar 31, 2012
 *      Author: irri
 */

#ifndef HILBERTCURVE_H_
#define HILBERTCURVE_H_

#include "../IRenderPattern.h"

namespace raytracer {

void rot(int n, int *x, int *y, int rx, int ry);
void d2xy(int n, int d, int *x, int *y);

class HilbertCurve: public raytracer::IRenderPattern {
public:
  HilbertCurve(int width, int height);
  virtual ~HilbertCurve();

  int* getBatch(int batchNr, int* length);
  int getNumberBatches();

private:
  int** batches;

  int n;
  int batch_size;
  int nr_of_batches;
  int width;
  int height;
};

}

#endif /* HILBERTCURVE_H_ */
