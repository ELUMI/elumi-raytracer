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

}
