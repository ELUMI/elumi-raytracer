/*
 * SimpleTracer.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: david
 */

#include <iostream>

#include "SimpleTracer.h"
#include <stdint.h>

#include <glm/glm.hpp>

using namespace glm;

namespace raytracer {

//ITracer::~ITracer() {}

SimpleTracer::SimpleTracer(Scene* scene, vec4 background_color) {
  SimpleTracer::scene = scene;
  SimpleTracer::background_color = background_color;
}

SimpleTracer::~SimpleTracer() {

}

void SimpleTracer::trace(Ray* rays, int length, uint8_t* buffer) {

  SimpleTracer::buffer = buffer;

  for (size_t i=0; i<length; ++i) {
    vec4 c = traceHelper(&rays[i]);

    buffer[i*4] = glm::min(255, int(c.r));
    buffer[i*4 +1] = glm::min(255, int(c.g));
    buffer[i*4 +2] = glm::min(255, int(c.b));
    buffer[i*4 +3] = glm::min(255, int(c.a));
  }
}

vec4 SimpleTracer::traceHelper(Ray* ray, int levels) {
  if (levels <= 0)
    return vec4(0,0,0,255);

  vec4 color;
  vec4 new_color;

  IAccDataStruct::IntersectionData intersection_data = scene->getAccDataStruct()->findClosestIntersection(*ray);
  if (intersection_data.triangle == NULL) {
    color = background_color;
  } else {

    //Light
    ILight* light = scene->getLightVector().front();
    Ray lightRay = Ray::generateRay(intersection_data.interPoint,light->getPosition());
    IAccDataStruct::IntersectionData intersection_data_light = scene->getAccDataStruct()->findClosestIntersection(lightRay);
    if (false) {
       color = background_color;
     } else {
       //Diffuse
       float diff = abs(dot(lightRay.getDirection(), intersection_data.normal));
       float falloff = light->getIntensity(length(light->getPosition()-intersection_data.interPoint));

       //Specular
       Ray refl = ray->reflection(*ray, intersection_data.normal, intersection_data.interPoint);

       float spec = glm::pow( dot(refl.getDirection(),light->getPosition()-intersection_data.interPoint),0.3f);
       spec = glm::max(1.0f,spec);

       color  =  spec * diff * falloff * intersection_data.triangle->getMaterial()->getColor();


       color.a = 255;
       //cout << dot(lightRay.getDirection(), intersection_data.normal) << endl;
     }
    //color  = intersection_data.triangle->getMaterial()->getColor();
    //SHADER
    //std::cout << intersection_data.triangle << "\n\n";

    //color = vec4(255,0,0,255);
    //Ray refl = ray->reflection(*ray, intersection_data.normal, intersection_data.interPoint);
    //new_color = traceHelper( &refl );
  }

//  color.r += int(0.5f * new_color.r);
//  color.g += int(0.5f * new_color.g);
//  color.b += int(0.5f * new_color.b);
  //color.a += int(0.5f * new_color.a);
  return color;
}

}
