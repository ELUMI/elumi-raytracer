/*
 * Random.cpp
 *
 *  Created on: 19 mar 2012
 *      Author: irri
 */

#include "Random.h"

namespace raytracer {

void init_generator() {
  generator.seed(static_cast<unsigned int>(std::time(0)));
}

float gen_random_float(float min, float max) {
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

}
