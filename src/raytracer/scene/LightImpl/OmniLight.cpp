/*
 * OmniLight.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: david
 */

#include "OmniLight.h"
#include "../../utilities/Random.h"

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

void OmniLight::setIntensity(float intensity) {
  m_intensity = intensity;
}

void OmniLight::setPosition(vec3 position) {
  m_position = position;
}

float OmniLight::getIntensity(float distance) {
  switch (m_falloff_type) {
    case NONE:
      return m_intensity;
      break;
    case LINEAR:
      //return min((1/distance)*m_intensity,1.0f);
      return (1.0f/distance) * m_intensity;
    case QUADRATIC:
      // return min((1/(distance*distance))*m_intensity,1.0f);
      return (1.0f/(distance*distance)) * m_intensity;
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

float OmniLight::distanceToBlocker(IAccDataStruct* datastruct, vec3 point, vec3 offset){
  Ray light_ray = Ray::generateRay(m_position+offset, point);
  IAccDataStruct::IntersectionData light_idata = datastruct->findClosestIntersection(light_ray);

  float distance_to_light = length(point - (m_position+offset));
  float distance_between_light_and_first_hit = length(light_idata.interPoint - (m_position+offset));

  return distance_to_light - distance_between_light_and_first_hit;
}

void OmniLight::getRays(Ray* rays, size_t n){
  for(size_t i = 0; i<n; ++i){
    rays[i] = Ray(m_position, get_random_dir());
  }
}


float OmniLight::calcLight(IAccDataStruct* datastruct, vec3 point, vec3 offset) {
  if (isBlocked(datastruct, point, offset))
    return 0.0f;
  return getIntensity( length(point - (m_position)) );
}


}
