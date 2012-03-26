/*
 * IRenderPattern.h
 *
 *  Created on: 19 mar 2012
 *      Author: julian
 */

#ifndef IRENDERPATTERN_H_
#define IRENDERPATTERN_H_

#include "utilities/Ray.h"

namespace raytracer {

class IRenderPattern {
public:
  virtual ~IRenderPattern() {};

  //virtual float* run(float* color_bufferm, int length) = 0;
  virtual int* getBatch(int batchNr, int* length) = 0;
  virtual int getNumberBatches() = 0;
};

}


#endif /* IRENDERPATTERN_H_ */
