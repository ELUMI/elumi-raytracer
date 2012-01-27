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
	Ray(vec3 pos, vec3 dir);
	virtual ~Ray();

  vec3 getPos();
  vec3 getDir();

private:
  vec3 pos;
  vec3 dir;
};

}

#endif /* RAY_H_ */
