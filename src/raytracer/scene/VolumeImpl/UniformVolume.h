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
#include "../../IPhaseFunctor.h"

namespace raytracer {

class UniformVolume : public IVolume {
public:
  UniformVolume(OBB obb, float absorption, float scattering, float emission, IPhaseFunctor* pf);
  virtual ~UniformVolume();

  float getTau(vec3 from, vec3 to);
  OBB::IntervalData getInterval(Ray ray);
  float getEmission(vec3 pos, vec3 dir);
  float getPhase(vec3& w_in, vec3& w_out);
  float getScattering();

private:
  OBB obb;

  // Extinction coefficient = absorption + out-scattering
  float absorption;
  float scattering;

  float emission;
  IPhaseFunctor* pf;
};

} /* namespace raytracer */
#endif /* UNIFORMVOLUME_H_ */
