/*
 * GridPattern.h
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#ifndef GRIDPATTERN_H_
#define GRIDPATTERN_H_

#include "../ISuperSamplePattern.h"

namespace raytracer {

class GridPattern: public ISuperSamplePattern {
public:
  GridPattern(int size);
  virtual ~GridPattern();

  void newPixel();
};

} /* namespace raytracer */
#endif /* GRIDPATTERN_H_ */
