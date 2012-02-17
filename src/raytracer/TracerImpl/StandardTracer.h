/*
 * StandardTracer.h
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */

#ifndef STANDARDTRACER_H_
#define STANDARDTRACER_H_

#include "BaseTracer.h"

namespace raytracer {

class StandardTracer: public BaseTracer {
public:
  StandardTracer(Scene* scene, Settings* settings);
  virtual ~StandardTracer();

  virtual void traceImage(float* color_buffer);

private:
  vec4 trace(Ray ray, IAccDataStruct::IntersectionData idata);
  vec4 tracePrim(Ray ray, const unsigned int depth);
  vec4 shade(Ray ray, IAccDataStruct::IntersectionData idata, const unsigned int depth);

  unsigned int max_recursion_depth;
};

}

#endif /* STANDARDTRACER_H_ */
