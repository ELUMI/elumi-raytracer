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

void init_generators(int n_threads = 12); //be safe...
void delete_generators(); //be safe...
float gen_random_float(float min, float max, int thread_id);
float gen_random_float(int thread_id);
vec3 gen_random_dir(int thread_id);
vec3 gen_random_hemisphere(vec3 normal, int thread_id);
vec3 gen_random_cone(vec3 normal, float max_angle, int thread_id);

}

#endif /* RANDOM_H_ */
