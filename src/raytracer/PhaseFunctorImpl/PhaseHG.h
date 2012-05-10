/*
 * PhaseHG.h
 *
 *  Created on: 17 apr 2012
 *      Author: julian
 */

#ifndef PHASEHG_H_
#define PHASEHG_H_

#include "../IPhaseFunctor.h"
#include "glm/glm.hpp"

namespace raytracer {

using namespace glm;

class PhaseHG : public IPhaseFunctor{
public:
  PhaseHG(float g = 0);
  float p(vec3& w_in, vec3& w_out);

private:
  float g;
};

} /* namespace raytracer */
#endif /* PHASEHG_H_ */
