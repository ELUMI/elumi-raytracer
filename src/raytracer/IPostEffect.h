/*
 * IPostEffect.h
 *
 *  Created on: 22 feb 2012
 *      Author: julian
 */

#ifndef IPOSTEFFECT_H_
#define IPOSTEFFECT_H_

#include <string>
#include <map>

namespace raytracer {

using namespace std;

class IPostEffect {
public:
  virtual ~IPostEffect() {};

  virtual float* run(float* color_bufferm, int length) = 0;

  static void registerEffect(string name, IPostEffect* effect) {
    effects[name] = effect;
  }

  static IPostEffect* getEffect(string name) {
    return effects[name];
  }

private:
  static map<string, IPostEffect*> effects;
};

}


#endif /* IPOSTEFFECT_H_ */
