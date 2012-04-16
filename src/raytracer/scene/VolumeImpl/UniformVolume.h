/*
 * UniformVolume.h
 *
 *  Created on: 10 apr 2012
 *      Author: julian
 */

#ifndef UNIFORMVOLUME_H_
#define UNIFORMVOLUME_H_

#include "../IVolume.h"
#include "../../utilities/OBB.h"

namespace raytracer {

class UniformVolume : public IVolume {
public:
  UniformVolume(OBB obb, float sigma_t, float emission);
  virtual ~UniformVolume();

  float getTau(vec3 from, vec3 to);
  OBB::IntervalData getInterval(Ray ray);
  float getEmission(vec3 pos, vec3 dir);

private:
  OBB obb;

  float sigma_t; // Extinction coefficient = absorption + out-scattering
  float emission;
};

} /* namespace raytracer */
#endif /* UNIFORMVOLUME_H_ */
