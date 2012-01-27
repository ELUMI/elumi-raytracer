/*
 * Ray.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "glm/glm.hpp"
#include "Ray.h"

namespace raytracer {



Ray::Ray(vec3 position, vec3 direction) {
  Ray::position = position;
  Ray::direction = direction;
}

Ray::~Ray() {
}

Ray Ray::generateRay(vec3 start_position, vec3 end_position) {
  vec3 direction = end_position - start_position;
  direction = normalize(direction);

  return Ray(start_position,direction);
}

const vec3& Ray::getPosition() {
  return position;
}

const vec3& Ray::getDirection() {
  return direction;
}

}
