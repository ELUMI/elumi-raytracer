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
  Ray::pos = position;
  Ray::dir = direction;
}

Ray::~Ray() {
}

vec3 Ray::getPos() {
  return position;
}

vec3 Ray::getDir() {
  return direction;
}

}
