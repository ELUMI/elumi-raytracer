/*
 * OmniLight.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: david
 */

#include "OmniLight.h"

namespace raytracer {

OmniLight::OmniLight() {
  m_falloff_type = NONE;
  m_position = vec3(0,0,0);
  m_intensity = 1.0f;
  color = vec3(1,1,1);
}

OmniLight::OmniLight(vec3 position) {
  OmniLight::m_position = position;
  m_intensity = 1.0f;
  m_falloff_type = NONE;
}

OmniLight::~OmniLight() {
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

vec3 OmniLight::getColor() const {
  return color;
}

void OmniLight::setDistanceFalloff(FalloffType falloff_type) {
  m_falloff_type = falloff_type;
}

void OmniLight::setColor(vec3 color) {
  this->color = color;
}

void OmniLight::draw() {
  GLUquadricObj *quadobj;
  quadobj = gluNewQuadric();
  //glColor3f(1.0f,1.0f,1.0f); //colors does not work
  //glMaterialfv(GL_FRONT,GL_DIFFUSE,farraygrey);
  gluSphere(quadobj, 0.5, 10,10);
  gluDeleteQuadric(quadobj);
}

}
