/*
 * JitterPattern.h
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#ifndef JITTERPATTERN_H_
#define JITTERPATTERN_H_

#include "../ISuperSamplePattern.h"

namespace raytracer {

class JitterPattern: public ISuperSamplePattern {
public:
  JitterPattern(int size, int thread_id);
  virtual ~JitterPattern();

  void newPixel();
private:
  int thread_id;
};

} /* namespace raytracer */
#endif /* JITTERPATTERN_H_ */
