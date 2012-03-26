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
  StandardTracer(Scene* scene);
  virtual ~StandardTracer();

  virtual void traceImage(float* color_buffer);

private:
  vec4 trace(Ray ray, IAccDataStruct::IntersectionData idata);
  vec4 tracePrim(Ray ray, float attenuation, unsigned short depth);
  vec4 shade(Ray ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth);

  const unsigned short MAX_RECURSION_DEPTH;
  const float ATTENUATION_THRESHOLD;
//  unsigned int recursion_depth;
//  float recursion_attenuation;
};

}

#endif /* STANDARDTRACER_H_ */
