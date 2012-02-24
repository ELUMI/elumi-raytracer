/*
 * ReinhardOperator.h
 *
 *  Created on: 22 feb 2012
 *      Author: julian
 */

#ifndef REINHARDOPERATOR_H_
#define REINHARDOPERATOR_H_

#include "../IPostEffect.h"

namespace raytracer {

class ReinhardOperator : public IPostEffect {

public:
  ReinhardOperator(float key = 0.18, float white = 1e20);

  //ReinhardOperator(float key = 0.18, float white = 1e20, int range, int low = 1, int high = 43);

private:
  float key;

};

}


#endif /* REINHARDOPERATOR_H_ */
