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

static boost::mt19937* generators = 0;

void init_generators(int n_threads) {
  generators = new boost::mt19937[n_threads];
  for(int i=0; i<n_threads; ++i) {
    generators[i] = boost::mt19937(42u);
    generators[i].seed(static_cast<unsigned int>(std::time(0)+i*37)); //some random extra seed
  }
}

void delete_generators(){
  if(generators)
    delete [] generators;
}

float gen_random_float(float min, float max, int thread_id) {
  if(min >= max) {
    return max;
  }
  boost::uniform_real<float> u(min, max);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen(generators[thread_id], u);
  return gen();
}

float gen_random_float(int thread_id) {
  return gen_random_float(0.0f, 1.0f, thread_id);
}

vec3 gen_random_dir(int thread_id) {
  float x, y, z, w, t;
  z = gen_random_float(-1.0f, 1.0f, thread_id);
  t = gen_random_float(0.0f, 2.0 * M_PI, thread_id);
  w = sqrt( 1 - z*z );
  x = w * cos( t );
  y = w * sin( t );
  return vec3(x,y,z);
}

vec3 gen_random_hemisphere(vec3 normal, int thread_id){
  vec3 dir = gen_random_dir(thread_id);
  if(dot(dir, normal) < 0)
    return -dir;
  return dir;
}

vec3 gen_random_cone(vec3 normal, float max, int thread_id) {
  //if(max <= 0.0f) // Cone as narrow as the axis
  //  return normal;

  if(max >= 2.0f * M_PI)  // Whole sphere, slightly faster
    return gen_random_dir(thread_id);

  // The algorithm assumes normal to point at vec3(0,0,1)
  float x, y, z, w, t;
  z = gen_random_float(cos(max), 1.0f, thread_id);
  t = gen_random_float(0.0f, 2.0 * M_PI, thread_id);
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
