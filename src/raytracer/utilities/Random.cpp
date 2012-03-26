/*
 * Random.cpp
 *
 *  Created on: 19 mar 2012
 *      Author: irri
 */

#include "Random.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace raytracer {

void init_generator() {
  generator.seed(static_cast<unsigned int>(std::time(0)));
}

float gen_random_float(float min, float max) {
  if(min >= max) {
    return max;
  }
  boost::uniform_real<float> u(min, max);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen(generator, u);
  return gen();
}

vec3 get_random_dir() {
  float x, y, z, w, t;
  z = gen_random_float(-1.0f, 1.0f);
  t = gen_random_float(0.0f, 2.0 * M_PI);
  w = sqrt( 1 - z*z );
  x = w * cos( t );
  y = w * sin( t );
  return vec3(x,y,z);
}

vec3 get_random_hemisphere(vec3 normal){
  vec3 dir = get_random_dir();
  if(dot(dir, normal) < 0)
    return -dir;
  return dir;
}

// TODO: Find better algorithm
vec3 get_random_cone(vec3 normal, float max) {
  //if(max <= 0.0f) // Cone as narrow as the axis
  //  return normal;

  if(max >= 2.0f * M_PI)  // Whole sphere, slightly faster
    return get_random_dir();

  // The algorithm assumes normal to point at vec3(0,0,1)
  float x, y, z, w, t;
  z = gen_random_float(cos(max), 1.0f);
  t = gen_random_float(0.0f, 2.0 * M_PI);
  w = sqrt( 1 - z*z );
  x = w * cos( t );
  y = w * sin( t );
  vec3 z_plus = vec3(x,y,z);

  // Transform to normal spaces
  vec3 local = vec3(0.0f, 0.0f, 1.0f);
  normal = normalize(normal);
  vec3 rot_axis = cross(normal, local);
  float ang = angle(normal, local);

  return rotate(z_plus, -ang, rot_axis);
//return normal;

//
//  // Find angle between normal and dir. If angle is outside conde, rotate
//  // back to within the cone.
//  float angle = acos(dot(dir, normal));
//  if(angle > max) {
//    float rot = ceil((angle - max) / (max * 2)) * max * 2;
//    return rotate(dir,  rot, cross(dir, normal));  // Rotate back within range
//  } else {
//    return dir;
//  }

}

}
