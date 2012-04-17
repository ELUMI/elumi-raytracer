/*
 * VolumeTracer.cpp
 *
 *  Created on: 10 apr 2012
 *      Author: julian
 */

#include "VolumeTracer.h"
#include "../scene/IVolume.h"

#include <iostream>

namespace raytracer {

VolumeTracer::VolumeTracer(Scene* scene) : StandardTracer(scene) {
  VolumeTracer::scene = scene;
}

VolumeTracer::~VolumeTracer() {
}

float VolumeTracer::getIndividualLight(vec3 pos, ILight* light_source, int thread_id) {
  float base_light = StandardTracer::getIndividualLight(pos, light_source, thread_id);
  float light = base_light;

  std::vector<IVolume*> volumes = scene->getVolumes();

  for(unsigned int i = 0; i < volumes.size(); i++) {
    IVolume* volume = volumes[i];

    Ray ray = Ray::generateRay(pos, light_source->getPosition());
    Interval inter = getInterval(ray, volume, light_source->getPosition());
    if(!inter.intersected)
      continue;
    vec3 min = inter.min;
    vec3 max = inter.max;
    if(glm::distance(pos, min) > glm::distance(pos, light_source->getPosition()))
      continue;
    light = glm::exp(-volume->getTau(min, max)) * base_light;
  }
  return light;
}

vec4 VolumeTracer::shade(Ray ray, IAccDataStruct::IntersectionData idata,
                         float attenuation, unsigned short depth, int thread_id) {

  vec4 base_color = StandardTracer::shade(ray, idata, attenuation, depth, thread_id);
  vec4 color = base_color;

  std::vector<IVolume*> volumes = scene->getVolumes();
  float step_size = scene->getSettings()->step_size;

  vec3 step = ray.getDirection() * step_size / length(ray.getDirection());

  // For each volume
  for(unsigned int i = 0; i < volumes.size(); i++) {

    IVolume* volume = volumes[i];

    Interval inter = getInterval(ray, volume, idata.interPoint);
    if(!inter.intersected)
      continue;
    vec3 min = inter.min;
    vec3 max = inter.max;

    float dist = glm::distance(min, max);

    // Add base color:
    color = glm::exp(-volume->getTau(min, max)) * base_color;

    int nr_steps = dist / step_size;
    float trans = 1.0f;
    for(int i = 0; i < nr_steps; ++i) {
      // Emission
      trans *= glm::exp(-volume->getTau(min + float(i)*step, min + (i+1.0f)*step));
      float Lve = volume->getEmission(min + (i+0.5f)*step, -step);
      std::cout << Lve << endl;
      color += trans * Lve * step_size;
    }

  }

  return color;
}

VolumeTracer::Interval VolumeTracer::getInterval(Ray ray, IVolume* volume, vec3 inter_point) {

  OBB::IntervalData intersect = volume->getInterval(ray);

  if(!intersect.intersected)
    return Interval();

  vec3 min = intersect.min;
  vec3 max = intersect.max;

  if(intersect.inside)  // If we're inside the box, we have no min, put ray position to min.
    min = ray.getPosition();

  float dist = glm::distance(min, max);

  if(glm::distance(min, inter_point) < dist)
    max = inter_point;

  return Interval(true, min, max);
}

} /* namespace raytracer */
