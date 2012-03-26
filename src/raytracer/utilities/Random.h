/*
 * Random.h
 *
 *  Created on: 19 mar 2012
 *      Author: irri
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <ctime>
#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <glm/glm.hpp>
using namespace glm;

namespace raytracer {

static boost::mt19937 generator(42u);
void init_generator();
float gen_random_float(float min, float max);
vec3 get_random_dir();
vec3 get_random_hemisphere(vec3 normal);

}

#endif /* RANDOM_H_ */
