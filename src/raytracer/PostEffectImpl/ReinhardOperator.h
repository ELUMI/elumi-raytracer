/*
 * ReinhardOperator.h
 *
 *  Created on: 22 feb 2012
 *      Author: julian
 */

#ifndef REINHARDOPERATOR_H_
#define REINHARDOPERATOR_H_

#include "../IPostEffect.h"
#include <glm/glm.hpp>

namespace raytracer {

class ReinhardOperator : public IPostEffect {

public:

  ReinhardOperator(float key = 0.18,    // Key value (~overall brightness).
                   float white = 1e20,  // White point (~energy of whitest point. Allows high luminaces to "burn out"
                   int range = 0,       // # iterations to find burning/dodging region. Zero equals do not use.
                   int low = 1,         // Smallest region in pixels.
                   int high = 43);      // Largest region in pixels.

  float* run(float* color_buffer, int lenght);

private:

  float logAverage(glm::vec3* buf, int length);
  float getMaxValue(glm::vec3* buf, int length);

  float key;
  float white;
  int range;
  int low;
  int high;
};

}


#endif /* REINHARDOPERATOR_H_ */
