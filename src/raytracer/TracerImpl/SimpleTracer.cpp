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

void SimpleTracer::trace(Ray* rays, int length/*, uint8_t* buffer*/) {
  for (size_t i=0; i<length; ++i) {
    Color color;
    try {
      IAccDataStruct::IntersectionData intersection_data = scene->getAccDataStruct()->findClosestIntersection(rays[i]);

      color = shade()
    } catch (NoIntersectionException e) {
      color = scene->getBackroundColor();
    }

  }
}

Color SimpleTracer::shade() {

}

}
