/*
 * Ray.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "glm/glm.hpp"
#include "Ray.h"

namespace raytracer {


Ray::Ray() {
  Ray::position = vec3(0,0,0);
  Ray::direction = vec3(0,0,1);
}

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

Ray Ray::reflection(Ray ray, vec3 normal, vec3 intersection_point) {
  vec3 r = ray.getDirection();

  vec3 refl = -r + 2.0f * glm::normalize(normal) * ( glm::dot(r,  normal) );
  return Ray(intersection_point, refl);
}

const vec3& Ray::getPosition() {
  return position;
}

const vec3& Ray::getDirection() {
  return direction;
}

}
