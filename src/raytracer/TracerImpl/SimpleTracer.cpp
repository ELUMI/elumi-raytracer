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
    buffer[i*4] = 255*glm::min(1.0f, c.r);
    buffer[i*4 +1] = 255*glm::min(1.0f, c.g);
    buffer[i*4 +2] = 255*glm::min(1.0f, c.b);
    buffer[i*4 +3] = 255*glm::min(1.0f, c.a);
  }
}

vec4 SimpleTracer::traceHelper(Ray* ray, int levels) {
  vec3 color_black = vec3(0.0f,0.0f,0.0f);

  IAccDataStruct::IntersectionData intersection_data = scene->getAccDataStruct()->findClosestIntersection(*ray);
  if (intersection_data.triangle == NULL) {
    return background_color;

  } else {

    vec3 color;
    //Light
    ILight* light = scene->getLightVector().front();
    Ray lightRay = Ray::generateRay(intersection_data.interPoint,light->getPosition());
    IAccDataStruct::IntersectionData intersection_data_light = scene->getAccDataStruct()->findClosestIntersection(lightRay);

    if (intersection_data_light.triangle == intersection_data.triangle ||
        length(light->getPosition()-intersection_data.interPoint) >
    length(light->getPosition()-intersection_data_light.interPoint)) {
      color = color_black;

    } else {

      //Diffuse
      float diff = abs(dot(lightRay.getDirection(), intersection_data.normal));
      float falloff = light->getIntensity(length(light->getPosition()-intersection_data.interPoint));

      //Specular
      Ray refl = ray->reflection(lightRay, intersection_data.normal, intersection_data.interPoint);
      vec3 v = normalize(ray->getPosition()-intersection_data.interPoint);

      float spec;
      if(dot(intersection_data.normal,lightRay.getDirection()) < 0) {
        spec = 0.0f;
      } else {
        //spec = glm::max(0.0f,glm::pow( dot(refl.getDirection(),ray->getPosition()-intersection_data.interPoint),0.3f));
        spec = glm::max(0.0f,glm::pow( dot(normalize(refl.getDirection()),v),1.0f));
      }

      vec3 white = vec3(1,1,1);

      color  = falloff * (1.0f * spec * white + diff * intersection_data.triangle->getMaterial()->getDiffuse());
      //color = falloff * diff * intersection_data.triangle->getMaterial()->getColor();
      // diff * intersection_data.triangle->getMaterial()->getColor()


      //cout << dot(lightRay.getDirection(), intersection_data.normal) << endl;

    }
    return vec4(color, intersection_data.triangle->getMaterial()->getTransparency());
  }
}

}
