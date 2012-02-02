/*
 * OmniLight.h
 *
 *  Created on: Feb 1, 2012
 *      Author: david
 */

#ifndef OMNILIGHT_H_
#define OMNILIGHT_H_

#include <glm/glm.hpp>

#include "../ILight.h"

using namespace glm;

namespace raytracer {

class OmniLight : public ILight {
public:
  OmniLight();
  OmniLight(vec3 position);
  virtual ~OmniLight();

  vec3 getPosition();
  float getIntensity(float distance);
  void setDistanceFalloff(FalloffType falloff_type);
  void setIntensity(float intesity);

private:
  vec3 m_position;
  float m_intensity;
  FalloffType m_falloff_type;
};

}

#endif /* OMNILIGHT_H_ */
