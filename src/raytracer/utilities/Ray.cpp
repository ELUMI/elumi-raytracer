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

Ray Ray::generateRay(vec3 startPosition, vec3 endPosition) {
  vec3 direction = endPostion - startPosition;
  direction = normalize(direction);

  return Ray(startPosition,direction);
}

vec3 Ray::getPosition() {
  return position;
}

vec3 Ray::getDirection() {
  return direction;
}

}
