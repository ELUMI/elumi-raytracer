/*
 * DebugTracer.cpp
 *
 *  Created on: Mar 14, 2012
 *      Author: ulvinge
 */

#include "DebugTracer.h"

namespace raytracer {

DebugTracer::DebugTracer(Scene* scene) : BaseTracer(scene) {
  // TODO Auto-generated constructor stub

}

DebugTracer::~DebugTracer() {
  // TODO Auto-generated destructor stub
}


vec4 DebugTracer::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata) {

  switch(settings->debug_mode) {
  case 1:
    return vec4(idata.material/5.0f,idata.material/10.0f,0,1);
  case 2:
    return vec4(idata.normal,1);
  case 3:
    return vec4(idata.normal*0.5f+0.5f,1);
  case 4:
    return vec4(scene->getMaterialVector()[idata.material]->getDiffuse(),1);
  case 5:
    return vec4(idata.texcoord,0,1);
  case 6: {
    IAccDataStruct::IntersectionData idata2 = scene->getAccDataStruct()->findClosestIntersection(incoming_ray);
    return vec4(normalize(idata.normal) - normalize(idata2.normal),1);
  }
  case 7: {
    IAccDataStruct::IntersectionData idata2 = scene->getAccDataStruct()->findClosestIntersection(incoming_ray);
    return vec4(abs(normalize(idata.normal) - normalize(idata2.normal))*100.0f,1);
  }
  case 8: {
    IAccDataStruct::IntersectionData idata2 = scene->getAccDataStruct()->findClosestIntersection(incoming_ray);
    return vec4(abs(idata.interPoint-idata2.interPoint)*10.0f,1);
  }
  case 9: {
    ILight* light = scene->getLightVector()->front();
    Ray lightRay = Ray::generateRay(light->getPosition(), idata.interPoint);
    IAccDataStruct::IntersectionData idatal = scene->getAccDataStruct()->findClosestIntersection(lightRay);
    return vec4(abs(idata.interPoint-idatal.interPoint),1);
  }
  case 10: {
    ILight* light = scene->getLightVector()->front();
    Ray lightRay = Ray::generateRay(light->getPosition(), idata.interPoint);
    IAccDataStruct::IntersectionData idatal = scene->getAccDataStruct()->findClosestIntersection(lightRay);

    IAccDataStruct::IntersectionData idata2 = scene->getAccDataStruct()->findClosestIntersection(incoming_ray);
    lightRay = Ray::generateRay(light->getPosition(), idata2.interPoint);
    IAccDataStruct::IntersectionData idatal2 = scene->getAccDataStruct()->findClosestIntersection(lightRay);
    return vec4(abs(idatal2.interPoint-idatal.interPoint),1);
  }

  }

  ILight* light = scene->getLightVector()->front();
  vec3 dir = normalize(idata.interPoint - light->getPosition());
  Ray lightRay = Ray(light->getPosition(), dir);
  IAccDataStruct::IntersectionData idatal = scene->getAccDataStruct()->findClosestIntersection(lightRay);
  //assert(idatal.material != IAccDataStruct::IntersectionData::NOT_FOUND);

  if (dot(dir,(idata.interPoint - idatal.interPoint)) > 0.001+settings->test) {
    return vec4();
  } else {
    Material* material = scene->getMaterialVector()[idata.material];
    return vec4(material->getDiffuse(),1);
  }

}

}
