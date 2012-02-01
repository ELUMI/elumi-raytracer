/*
 * SimpleTracer.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: david
 */

#include "SimpleTracer.h"
#include <stdint.h>

namespace raytracer {

//ITracer::~ITracer() {}

SimpleTracer::SimpleTracer(Scene* scene) {
  SimpleTracer::scene = scene;
}

SimpleTracer::~SimpleTracer() {

}

void SimpleTracer::trace(Ray* rays, int length, uint8_t* buffer) {

  SimpleTracer::buffer = buffer;

  for (size_t i=0; i<length; ++i) {
    Color c = traceHelper(&rays[i]);
    buffer[i*4] = max(255, int(c.r));
    buffer[i*4 +1] = max(255, int(c.g));
    buffer[i*4 +2] = max(255, int(c.b));
    buffer[i*4 +3] = max(255, int(c.a));
  }
}

Color SimpleTracer::traceHelper(Ray* ray, int levels) {

  if (levels <= 0)
    return Color();

  Color color;
  Color new_color;
  try {
    IAccDataStruct::IntersectionData intersection_data = scene->getAccDataStruct()->findClosestIntersection(*ray);

    //SHADER
    Color color  = intersection_data.triangle->getMaterial()->getColor();

    Ray refl = ray->reflection(*ray, intersection_data.normal, intersection_data.interPoint);
    new_color = traceHelper( &refl );

  } catch (NoIntersectionException e) {
    color = scene->getBackroundColor();
    new_color.r=0;
    new_color.g=0;
    new_color.b=0;
    new_color.a=0;
  }

  color.r += int(0.5f * new_color.r);
  color.g += int(0.5f * new_color.g);
  color.b += int(0.5f * new_color.b);
  color.a += int(0.5f * new_color.a);
  return color;
}

}
