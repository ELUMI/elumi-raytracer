/*
 * LinePattern.h
 *
 *  Created on: 19 mar 2012
 *      Author: julian
 */

#ifndef LINEPATTERN_H_
#define LINEPATTERN_H_

#include "../IRenderPattern.h"

namespace raytracer {

class LinePattern : public IRenderPattern {

public:
  LinePattern(int width, int heights);
  ~LinePattern();
  int* getBatch(int batchNr, int* length);
  int getNumberBatches();

private:
  int** batches;
  int width;
  int height;
};

}

#endif /* LINEPATTERN_H_ */
