/*
 * AreaLight.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: irri
 */

#include "AreaLight.h"

namespace raytracer {

AreaLight::AreaLight() {

}

AreaLight::~AreaLight() {

}

ILight::FalloffType AreaLight::getFalloffType() {
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

float AreaLight::distanceToBlocker(IAccDataStruct *datastruct, vec3 point) {
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
  return -1.0f;
}

}
