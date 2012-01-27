/*
 * Ray.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "glm/glm.hpp"
#include "Ray.h"

namespace raytracer {

Ray::Ray(vec3 pos, vec3 dir) {
  Ray::pos = pos;
  Ray::dir = dir;
}

Ray::~Ray() {
}

vec3 Ray::getPos() {
  return pos;
}

vec3 Ray::getDir() {
  return dir;
}

}
