/*
 * OmniLight.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: david
 */

#include "BaseLight.h"
#include "../../utilities/Random.h"

namespace raytracer {

BaseLight::BaseLight() {
  falloff_type = NONE;
  position = vec3(0,0,0);
  intensity = 1.0f;
  color = vec3(1,1,1);

  nr_of_caches = 0;
  cache = NULL;
}

BaseLight::BaseLight(vec3 position) {
  this->position = position;
  intensity = 1.0f;
  falloff_type = NONE;
  color = vec3(1,1,1);

  nr_of_caches = 0;
  cache = NULL;
}

BaseLight::~BaseLight() {
  if(cache!=NULL)
    delete [] cache;
}

vec3 BaseLight::getPosition() {
  return position;
}

void BaseLight::setIntensity(float intensity) {
  this->intensity = intensity;
}

void BaseLight::setPosition(vec3 position) {
  this->position = position;
}

float BaseLight::getIntensity() {
  return intensity;
}

float BaseLight::getFalloff(float distance) {
  switch (falloff_type) {
  case NONE:
  default:
    return 1.0f;
    break;
  case LINEAR:
    //return min((1/distance)*intensity,1.0f);
    return (1.0f/distance);
  case QUADRATIC:
    // return min((1/(distance*distance))*intensity,1.0f);
    return (1.0f/(distance*distance));
  }
}

ILight::FalloffType BaseLight::getFalloffType() const {
  return falloff_type;
}

vec3 BaseLight::getColor() const {
  return color;
}

void BaseLight::setDistanceFalloff(FalloffType falloff_type) {
  this->falloff_type = falloff_type;
}

void BaseLight::setColor(vec3 color) {
  this->color = color;
}

void BaseLight::draw() {
  GLUquadricObj *quadobj;
  quadobj = gluNewQuadric();
  //glColor3f(1.0f,1.0f,1.0f); //colors does not work
  //glMaterialfv(GL_FRONT,GL_DIFFUSE,farraygrey);
  glColor3f(color.r, color.g, color.b);
  gluSphere(quadobj, 0.5, 10,10);
  gluDeleteQuadric(quadobj);
}

float BaseLight::distanceToBlocker(IAccDataStruct* datastruct, vec3 point, int thread_id, vec3 offset){
  Ray light_ray = Ray::generateRay(position+offset, point);


  // Shadow cache
  if(cache!=NULL && cache[thread_id] != NULL) {
    if( IAccDataStruct::instersects(&light_ray, cache[thread_id]) )
      return 1.0f;
  }

  IAccDataStruct::IntersectionData light_idata = datastruct->findClosestIntersection(light_ray);


  float distance_to_light = length(point - (position+offset));
  float distance_between_light_and_first_hit = length(light_idata.interPoint - (position+offset));
  float dtb = distance_to_light - distance_between_light_and_first_hit;

  // Shadow cache
  if(cache!=NULL && dtb > EPSILON)
    cache[thread_id] = light_idata.triangle;

  return dtb;
}

void BaseLight::getRays(Ray* rays, size_t n, int thread_id){
  for(size_t i = 0; i<n; ++i){
    rays[i] = Ray(position, gen_random_dir(thread_id));
  }
}

float BaseLight::calcLight(IAccDataStruct* datastruct, vec3 point, int thread_id, vec3 offset) {
  if (isBlocked(datastruct, point, thread_id, offset))
    return 0.0f;
  return getIntensity() * getFalloff( length(point - (position)) );
}

void BaseLight::initCaches(size_t nr_of_threads) {
  nr_of_caches = nr_of_threads;
  cache = new Triangle*[nr_of_threads];
  for(size_t i=0; i<nr_of_threads; ++i)
    cache[i]=NULL;
}

}
