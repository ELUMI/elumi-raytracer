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

Ray::getPos() {
  return pos;
}

Ray::getDir() {
  return dir;
}

}
