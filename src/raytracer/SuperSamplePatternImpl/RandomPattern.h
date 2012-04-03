/*
 * RandomPattern.h
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#ifndef RANDOMPATTERN_H_
#define RANDOMPATTERN_H_

#include "../ISuperSamplePattern.h"

namespace raytracer {

class RandomPattern: public ISuperSamplePattern {
public:
  RandomPattern(int size, int thread_id);
  virtual ~RandomPattern();

  void newPixel();
  int thread_id;
};

} /* namespace raytracer */
#endif /* RANDOMPATTERN_H_ */
