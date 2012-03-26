/*
 * GammaEncode.h
 *
 *  Created on: 26 feb 2012
 *      Author: julian
 */

#ifndef GAMMAENCODE_H_
#define GAMMAENCODE_H_

#include "../IPostEffect.h"

namespace raytracer {

class GammaEncode : public IPostEffect {

public:

  GammaEncode(float gamma = 2.22f);

  void run(float* color_buffer, int pixels, int channels);

private:
  float gamma;

};

}


#endif /* GAMMAENCODE_H_ */
