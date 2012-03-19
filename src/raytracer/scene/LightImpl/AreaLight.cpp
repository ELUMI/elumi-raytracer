/*
 * AreaLight.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: irri
 */

#include "AreaLight.h"

namespace raytracer {

AreaLight::AreaLight() {
  position = vec3(0,0,0);
  axis1 = vec3(1.0f, 0.0f, 0.0f);
  axis2 = vec3(0.0f, 0.0f, 1.0f);
  sample1 = sample2 = 3;

  light_sources = new OmniLight[sample1*sample2];

  falloff_type = QUADRATIC;
  intensity = 1.0f;
  color = vec3(1,1,1);
}

AreaLight::AreaLight(vec3 position, vec3 axis1, vec3 axis2, unsigned int sample1, unsigned int sample2) {
  this->position = position;
  this->axis1 = axis1;
  this->axis2 = axis2;
  this->sample1 = sample1;
  this->sample2 = sample2;

  falloff_type = QUADRATIC;
  intensity = 1.0f;
  color = vec3(1,1,1);

  light_sources = new OmniLight[sample1*sample2];
  vec3 top_left = position - 0.5f*axis1 - 0.5f*axis2;
  vec3 delta1 = axis1 * (1.0f/sample1);
  vec3 delta2 = axis2 * (1.0f/sample2);

  for (unsigned int x=0; x<sample1; ++x) {
    for (unsigned int y=0; y<sample2; ++y) {

      vec3 sample_pos = top_left + (float)x*delta1 + (float)y*delta2;
      light_sources->setPosition(sample_pos);
      light_sources->setDistanceFalloff(falloff_type);
    }
  }

}

AreaLight::~AreaLight() {
  delete light_sources;
}

ILight::FalloffType AreaLight::getFalloffType() const {
  return falloff_type;
}

void AreaLight::setDistanceFalloff(FalloffType falloff_type) {
  this->falloff_type = falloff_type;
}

vec3 AreaLight::getPosition() {
  return position;
}

float AreaLight::getIntensity(float distance) {
  //TODO
  return 1.0f;
}

void AreaLight::setIntensity(float intensity) {
  this->intensity = intensity;
}

void AreaLight::setColor(vec3 color) {
  this->color = color;
}

void AreaLight::draw() {
  GLUquadricObj *quadobj;
  quadobj = gluNewQuadric();
  //glColor3f(1.0f,1.0f,1.0f); //colors does not work
  //glMaterialfv(GL_FRONT,GL_DIFFUSE,farraygrey);
  //glColor3f(color.r, color.g, color.b);
  glColor3f(0.0f, 0.0f, 1.0f);
  gluSphere(quadobj, 0.5, 10,10);
  gluDeleteQuadric(quadobj);
}

vec3 AreaLight::getColor() const {
  return color;
}

void AreaLight::setPosition(vec3 position) {
  this->position = position;
}

float AreaLight::calcLight(IAccDataStruct* datastruct, vec3 point) {
  float light = 0.0f;

  for (unsigned int x=0; x<sample1; ++x) {
    for (unsigned int y=0; y<sample2; ++y) {



    }
  }

  return light;
}

}
