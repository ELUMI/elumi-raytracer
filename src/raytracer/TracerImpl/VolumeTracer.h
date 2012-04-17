/*
 * VolumeTracer.h
 *
 *  Created on: 10 apr 2012
 *      Author: julian
 */

#ifndef VOLUMETRACER_H_
#define VOLUMETRACER_H_

#include "PhotonMapper.h"
#include "../scene/Scene.h"

namespace raytracer {

class VolumeTracer: public raytracer::StandardTracer {
public:
  VolumeTracer(Scene* scene);
  virtual ~VolumeTracer();

protected:
  vec4 shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth, int thread_id);

private:
  Scene* scene;
};

} /* namespace raytracer */
#endif /* VOLUMETRACER_H_ */
