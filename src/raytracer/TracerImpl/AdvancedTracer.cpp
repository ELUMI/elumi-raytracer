/*
 * AdvancedTracer.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: ulvinge
 */

#include "AdvancedTracer.h"

namespace raytracer {

AdvancedTracer::AdvancedTracer(Scene* scene) : PhotonMapper(scene) {
}

AdvancedTracer::~AdvancedTracer() {
}

void AdvancedTracer::tracePhoton(Photon p)
{
  if(!bounce(p,false)) //first bounce should not be saved
    return;
  for(size_t k = 0;k < settings->max_recursion_depth;++k){
    if(abort)
      break;

    if(!bounce(p))
      break;
  }
}

vec4 AdvancedTracer::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short  depth){
  return StandardTracer::shade(incoming_ray, idata, attenuation, depth);
}


}
