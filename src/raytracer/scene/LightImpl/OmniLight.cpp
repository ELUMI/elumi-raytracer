/*
 * OmniLight.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: david
 */

#include "OmniLight.h"

namespace raytracer {

OmniLight::OmniLight() {
  // TODO Auto-generated constructor stub
  m_falloff_type = NONE;
  m_position = vec3(0,0,0);
  m_intensity = 1.0f;
}

OmniLight::OmniLight(vec3 position) {
  // TODO Auto-generated constructor stub
  OmniLight::m_position = position;
  m_intensity = 1.0f;
  m_falloff_type = NONE;
}

OmniLight::~OmniLight() {
  // TODO Auto-generated destructor stub
}

vec3 OmniLight::getPosition() {
  return m_position;
}

void OmniLight::setIntensity(float intensity) {
  m_intensity = intensity;
}

float OmniLight::getIntensity(float distance) {
  switch (m_falloff_type) {
    case NONE:
      return 1.0f;
      break;
    case LINEAR:
      return min((1/distance)*m_intensity,1.0f);
    case QUADRATIC:
      return min((1/(distance*distance))*m_intensity,1.0f);
    default:
      return 1.0f;
      break;
  }
}

void OmniLight::setDistanceFalloff(FalloffType falloff_type) {
  m_falloff_type = falloff_type;
}

}
