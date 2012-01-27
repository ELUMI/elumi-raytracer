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

	static Ray generateRay(vec3 start_position, vec3 end_position);

  const vec3& getPosition();
  const vec3& getDirection();

private:
  vec3 position;
  vec3 direction;
};

}

#endif /* RAY_H_ */
