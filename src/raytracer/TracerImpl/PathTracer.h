/*
 * PathTracer.h
 *
 *  Created on: Apr 2, 2012
 *      Author: idk
 */

#ifndef PATHTRACER_H_
#define PATHTRACER_H_

#include "StandardTracer.h"

namespace raytracer {

class PathTracer: public raytracer::StandardTracer {
public:
	PathTracer(Scene* scene);
	virtual ~PathTracer();

  virtual vec4 shade(Ray ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth, int thread_id);
};

} /* namespace raytracer */
#endif /* PATHTRACER_H_ */
