/*
 * AreaLight.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: irri
 */

#include "AreaLight.h"
#include "../../utilities/Random.h"

namespace raytracer {

AreaLight::AreaLight() {
  axis1 = vec3(1.0f, 0.0f, 0.0f);
  axis2 = vec3(0.0f, 0.0f, 1.0f);
  sample1 = sample2 = 3;
  samples = sample1*sample2;

  falloff_type = QUADRATIC;
  intensity = 1.0f;
  color = vec3(1,1,1);

  light_sources = new BaseLight[sample1*sample2];
  setPosition(vec3(0,0,0));
  setDistanceFalloff(falloff_type);
}

AreaLight::AreaLight(vec3 position, vec3 axis1, vec3 axis2, unsigned int sample1, unsigned int sample2) {
  this->position = position;
  this->axis1 = axis1;
  this->axis2 = axis2;
  this->sample1 = sample1;
  this->sample2 = sample2;
  samples = sample1*sample2;
  delta1 = vec3(0.0f,0.0f,0.0f);
  delta2 = vec3(0.0f,0.0f,0.0f);

  falloff_type = QUADRATIC;
  intensity = 1.0f;
  color = vec3(1,1,1);

  light_sources = new BaseLight[samples];
  setPosition(position);
  setDistanceFalloff(falloff_type);


}

AreaLight::~AreaLight() {
  delete [] light_sources;
}

ILight::FalloffType AreaLight::getFalloffType() const {
  return falloff_type;
}

void AreaLight::setDistanceFalloff(FalloffType falloff_type) {
  this->falloff_type = falloff_type;

  BaseLight* light = light_sources;
  for (unsigned int i=0; i<samples; ++i,light++) {
    light->setDistanceFalloff(falloff_type);
  }
}

float AreaLight::getFalloff(float distance) {
  //TODO
  return 1.0f;
}

void AreaLight::setIntensity(float intensity) {
  this->intensity = intensity;

  BaseLight* light = light_sources;
  for (unsigned int i=0; i<samples; ++i,light++) {
    light->setIntensity(intensity);
  }
}

void AreaLight::setColor(vec3 color) {
  this->color = color;
  BaseLight* light = light_sources;
  for (unsigned int i=0; i<samples; ++i,light++) {
    light->setColor(color);
  }
}

void AreaLight::draw() {
  BaseLight* light = light_sources;
  for (unsigned int i=0; i<samples; ++i,light++) {
    light->draw();
  }
}


void AreaLight::setPosition(vec3 position) {
  this->position = position;

  vec3 top_left = position - 0.5f*axis1 - 0.5f*axis2;
  delta1 = axis1 * (1.0f/sample1);
  delta2 = axis2 * (1.0f/sample2);

  BaseLight* light = light_sources;
  for (unsigned int x=0; x<sample1; ++x) {
    for (unsigned int y=0; y<sample2; ++y) {
      vec3 sample_pos = top_left + (float)x*delta1 + (float)y*delta2;
      light->setPosition(sample_pos);
      light++;
    }
  }
}

void AreaLight::getRays(Ray* rays, size_t n, int thread_id) {
  for(size_t i = 0; i<n; ++i){
    size_t j = gen_random_float(0.0f, samples, thread_id);
    Ray ray;
    light_sources[j].getRays(&ray,1,thread_id);
    vec3 offset = gen_random_float(thread_id) * delta1 + gen_random_float(thread_id) * delta2;
    rays[i] = Ray(ray.getPosition()+offset, ray.getDirection());
  }
}


float AreaLight::calcLight(IAccDataStruct* datastruct, vec3 point, int thread_id, vec3 area_offset) {
  float in_light = 0.0f;

  BaseLight* light = light_sources;
  for (unsigned int i=0; i<samples; ++i,light++) {
    vec3 offset = area_offset
                + gen_random_float(0.0f, 1.0f, thread_id) * delta1
                + gen_random_float(0.0f, 1.0f, thread_id) * delta2;
    in_light += light->calcLight(datastruct, point, thread_id, offset) / samples;
  }

  return in_light;
}

void AreaLight::initCaches(size_t nr_of_threads) {
  BaseLight* light = light_sources;
  for (unsigned int i=0; i<samples; ++i,light++) {
    light->initCaches(nr_of_threads);
  }
}

}
