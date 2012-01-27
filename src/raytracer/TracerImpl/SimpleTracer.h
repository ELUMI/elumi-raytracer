/*
 * SimpleTracer.h
 *
 *  Created on: Jan 26, 2012
 *      Author: david
 */

#ifndef SIMPLETRACER_H_
#define SIMPLETRACER_H_

#include "../ITracer.h"

namespace raytracer {

class SimpleTracer : public ITracer {
public:
  SimpleTracer();
  virtual ~SimpleTracer();

  void trace(Ray* ray, unsigned char* buffer);
private:
  void shade();
};

}

#endif /* SIMPLETRACER_H_ */
