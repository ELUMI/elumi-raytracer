/*
 * IPostEffect.h
 *
 *  Created on: 22 feb 2012
 *      Author: julian
 */

#ifndef IPOSTEFFECT_H_
#define IPOSTEFFECT_H_

namespace raytracer {

class IPostEffect {
public:
  virtual ~IPostEffect() {};

  virtual float* run(float* color_bufferm, int length) = 0;

};

}


#endif /* IPOSTEFFECT_H_ */
