/*
 * AdvancedTracer.h
 *
 *  Created on: Mar 26, 2012
 *      Author: ulvinge
 */

#ifndef ADVANCEDTRACER_H_
#define ADVANCEDTRACER_H_

#include "PhotonMapper.h"

namespace raytracer {

class AdvancedTracer: public PhotonMapper {
public:
  AdvancedTracer(Scene* scene);
  virtual ~AdvancedTracer();

protected:
  virtual void tracePhoton(Photon p, int thread_id);
  vec4 shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth, int thread_id);
};

}

#endif /* ADVANCEDTRACER_H_ */
