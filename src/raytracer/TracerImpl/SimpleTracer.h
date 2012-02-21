/*
 * SimpleTracer.h
 *
 *  Created on: Jan 26, 2012
 *      Author: david
 */

#ifndef SIMPLETRACER_H_
#define SIMPLETRACER_H_

#include "BaseTracer.h"

namespace raytracer {

class SimpleTracer : public BaseTracer {
public:
  SimpleTracer(Scene* scene, Settings* settings);
  virtual ~SimpleTracer();

  void traceImage(float* color_buffer);
private:
  void createRays();
  vec4 traceHelper(Ray* ray, IAccDataStruct::IntersectionData idata);
};

}

#endif /* SIMPLETRACER_H_ */
