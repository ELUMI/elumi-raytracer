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
  vec3 l = ray.getDirection();
  vec3 refl = glm::reflect(l, normal);
  //vec3 refl = glm::normalize( -l + 2.0f * normal * ( glm::dot(l,  normal)) );
  return Ray(intersection_point, refl);
}

Ray Ray::refraction(Ray ray, vec3 n, vec3 intersection_point, float index) {
  vec3 i = ray.direction;
//  float cos = glm::dot(-n, i);
 // vec3 refr = glm::normalize(index*i + (index*cos - sqrt( 1 - index*index * ( 1 -cos*cos) ) ) * n);
  vec3 refr = glm::refract(i,n,index);

  return Ray(intersection_point, refr);
}

const vec3& Ray::getPosition() {
  return position;
}

const vec3& Ray::getDirection() {
  return direction;
}

}
