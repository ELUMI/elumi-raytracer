/*
 * IVolume.h
 *
 *  Created on: 10 apr 2012
 *      Author: julian
 */

#ifndef IVOLUME_H_
#define IVOLUME_H_

#include "glm/glm.hpp"
#include "../utilities/OBB.h"

namespace raytracer {

using namespace glm;

class IVolume {
public:

  virtual ~IVolume() {};

  virtual OBB::IntervalData getInterval(Ray ray) = 0;
  virtual float getTau(vec3 from, vec3 to) = 0;
  virtual float getEmission(vec3 pos, vec3 dir) = 0;
  virtual float getPhase(vec3& w_in, vec3& w_out) = 0;
  virtual float getScattering() = 0;

};

} /* namespace raytracer */
#endif /* IVOLUME_H_ */
