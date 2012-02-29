/*
 * IPostEffect.h
 *
 *  Created on: 22 feb 2012
 *      Author: julian
 */

#ifndef IPOSTEFFECT_H_
#define IPOSTEFFECT_H_

#include <string>
#include <queue>

namespace raytracer {

using namespace std;

class IPostEffect {
public:
  virtual ~IPostEffect() {};

  virtual float* run(float* color_bufferm, int length) = 0;

  static void registerEffect(string name, IPostEffect* effect) {
    effects.push(effect);
  }

  static IPostEffect* getEffect(string name) {
    if(!effects.empty()) {
      IPostEffect* effect = effects.front();
      effects.pop();
      return effect;
    } else {
      return NULL;
    }
  }

private:
  static queue<IPostEffect*> effects; // TODO Fix complete dynamic loading.
};

}


#endif /* IPOSTEFFECT_H_ */
