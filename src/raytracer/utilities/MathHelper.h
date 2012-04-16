/*
 * MathHelper.h
 *
 *  Created on: Mar 27, 2012
 *      Author: david
 */

#ifndef MATHHELPER_H_
#define MATHHELPER_H_

#include "glm/glm.hpp"

using namespace glm;

namespace raytracer {

class MathHelper {
public:
  MathHelper();
  virtual ~MathHelper();

  float lengthOfVector(vec3 vector); //Because glms function does not work as expected...
};

}

#endif /* MATHHELPER_H_ */
