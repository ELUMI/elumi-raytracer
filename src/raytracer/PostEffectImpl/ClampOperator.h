/*
 * ClampOperator.h
 *
 *  Created on: 26 feb 2012
 *      Author: julian
 */

#ifndef CLAMPOPERATOR_H_
#define CLAMPOPERATOR_H_

#include "../IPostEffect.h"

namespace raytracer {

class ClampOperator : public IPostEffect {

public:

  ClampOperator();

  float* run(float* color_buffer, int lenght);

private:

};

}


#endif /* CLAMPOPERATOR_H_ */
