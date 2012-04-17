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
  float getIndividualLight(vec3 pos, ILight* light, int thread_id);

private:

  struct Interval {
    Interval(bool intersected = false, vec3 min = vec3(), vec3 max = vec3()) {
      Interval::intersected = intersected;
      Interval::min = min;
      Interval::max = max;
    }
    bool intersected;
    vec3 min;
    vec3 max;
  };

  Scene* scene;

  Interval getInterval(Ray ray, IVolume* volume, vec3 inter_point);
};

} /* namespace raytracer */
#endif /* VOLUMETRACER_H_ */
