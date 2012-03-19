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
  color = vec3(1,1,1);
}

OmniLight::~OmniLight() {
}

vec3 OmniLight::getPosition() {
  return m_position;
}

void OmniLight::setPosition(vec3 position) {
  m_position = position;
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
      return glm::min((1/distance)*m_intensity,1.0f);
    case QUADRATIC:
      return glm::min((1/(distance*distance))*m_intensity,1.0f);
    default:
      return 1.0f;
      break;
  }
}

ILight::FalloffType OmniLight::getFalloffType() const {
  return m_falloff_type;
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
  glColor3f(color.r, color.g, color.b);
  gluSphere(quadobj, 0.5, 10,10);
  gluDeleteQuadric(quadobj);
}

float OmniLight::distanceToBlocker(IAccDataStruct* datastruct, vec3 point){
  Ray light_ray = Ray::generateRay(m_position, point);
  IAccDataStruct::IntersectionData light_idata = datastruct->findClosestIntersection(light_ray);

  float distance_to_light = length(point - getPosition());
  float distance_between_light_and_first_hit = length(light_idata.interPoint - m_position);

  return distance_to_light - distance_between_light_and_first_hit;
}


float OmniLight::calcLight(IAccDataStruct* datastruct, vec3 point) {
  if (isBlocked(datastruct, point))
    return 0.0f;
  return getIntensity( length(point - getPosition()) );
}


}
