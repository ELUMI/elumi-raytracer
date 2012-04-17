/*
 * PhaseSchlick.h
 *
 *  Created on: 17 apr 2012
 *      Author: julian
 */

#ifndef PHASESCHLICK_H_
#define PHASESCHLICK_H_

#include "../IPhaseFunctor.h"
#include "glm/glm.hpp"

namespace raytracer {

using namespace glm;

class PhaseSchlick : public IPhaseFunctor {
public:
  PhaseSchlick(float k = 0);
  ~PhaseSchlick() {};
  float p(vec3& w_in, vec3& w_out);

  // Helpfunction to get K as a function from G
  static float HGtoSchlick(float g) {
    return (1.55f * g - 0.55f * g*g*g);
  }

private:
  float k;
};

} /* namespace raytracer */
#endif /* PHASESCHLICK_H_ */
