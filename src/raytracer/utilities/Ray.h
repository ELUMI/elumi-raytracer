/*
 * Ray.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef RAY_H_
#define RAY_H_

#include "glm/glm.hpp"

using namespace glm;

namespace raytracer {

class Ray {
public:
	Ray(vec3 position, vec3 direction);
	virtual ~Ray();

  vec3 getPosition();
  vec3 getDirection();

private:
  vec3 position;
  vec3 direction;
};

}

#endif /* RAY_H_ */
