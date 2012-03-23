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

  void run(float* color_buffer, int pixels, int channels);

private:

};

}


#endif /* CLAMPOPERATOR_H_ */
