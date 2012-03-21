/*
 * DebugTracer.h
 *
 *  Created on: Mar 14, 2012
 *      Author: ulvinge
 */

#ifndef DEBUGTRACER_H_
#define DEBUGTRACER_H_

#include "BaseTracer.h"

namespace raytracer {

class DebugTracer: public BaseTracer {
public:
  DebugTracer(Scene* scene, Settings* settings);
  virtual ~DebugTracer();

  vec4 shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata);

};

}

#endif /* DEBUGTRACER_H_ */
