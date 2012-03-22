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


namespace raytracer {

class Random {
public:
  Random();
  virtual ~Random();

  float gen_random_float(float min, float max);

private:
  boost::mt19937 generator;
};

}

#endif /* RANDOM_H_ */
