/*
 * IPhaseFunctor.h
 *
 *  Created on: 17 apr 2012
 *      Author: julian
 */

#ifndef IPHASEFUNCTOR_H_
#define IPHASEFUNCTOR_H_

#include "glm/glm.hpp"

namespace raytracer {

using namespace glm;

class IPhaseFunctor {
public:
  virtual ~IPhaseFunctor() {};
  virtual float p(vec3& w_in, vec3& w_out) = 0;
};

} /* namespace raytracer */
#endif /* IPHASEFUNCTOR_H_ */
