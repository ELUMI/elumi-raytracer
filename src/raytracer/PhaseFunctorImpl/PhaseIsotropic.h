/*
 * PhaseIsotropic.h
 *
 *  Created on: 17 apr 2012
 *      Author: julian
 */

#ifndef PHASEISOTROPIC_H_
#define PHASEISOTROPIC_H_

#include "../IPhaseFunctor.h"

#include "glm/glm.hpp"

namespace raytracer {

using namespace glm;

class PhaseIsotropic : public IPhaseFunctor {
public:
  float p(vec3& w_in, vec3& w_out);
};

} /* namespace raytracer */
#endif /* PHASEISOTROPIC_H_ */
