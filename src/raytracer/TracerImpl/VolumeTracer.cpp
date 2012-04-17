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

vec4 VolumeTracer::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata,
                         float attenuation, unsigned short depth, int thread_id) {

  vec4 base_color = StandardTracer::shade(incoming_ray, idata, attenuation, depth, thread_id);
  vec4 color = base_color;

  std::vector<IVolume*> volumes = scene->getVolumes();
  float step_size = scene->getSettings()->step_size;

  vec3 step = incoming_ray.getDirection() * step_size / length(incoming_ray.getDirection());

  // For each volume
  for(int i = 0; i < volumes.size(); i++) {

    IVolume* volume = volumes[i];

    OBB::IntervalData intersect = volume->getInterval(incoming_ray);

    if(!intersect.intersected)
      continue;   // Continue to next volume if no intersection

    vec3 min = intersect.min;
    vec3 max = intersect.max;

    if(intersect.inside)  // If we're inside the box, we have no min, put ray position to min.
      min = incoming_ray.getPosition();

    float dist = glm::distance(min, max);

    vec3 closest_pos = max;

    if(glm::distance(min, idata.interPoint) < dist)
      closest_pos = idata.interPoint;

    // Add base color:
    color = glm::exp(-volume->getTau(min, closest_pos)) * base_color;

    int nr_steps = dist / step_size;
    for(int i = 0; i < nr_steps; ++i) {

    }

  }

  return color;
}

} /* namespace raytracer */
