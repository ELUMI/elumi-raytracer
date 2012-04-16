/*
 * CombinedDataStruct.cpp
 *
 *  Created on: Apr 9, 2012
 *      Author: idk
 */

#include "CombinedDataStruct.h"

namespace raytracer {

CombinedDataStruct::CombinedDataStruct() {
}

CombinedDataStruct::~CombinedDataStruct() {
  for(int i=0;i<datastructs.size();++i){
    delete datastructs[i];
  }
}

IAccDataStruct::IntersectionData CombinedDataStruct::findClosestIntersection(Ray ray){
  IAccDataStruct::IntersectionData closest = IntersectionData::miss();
  float closestd;
  for(int i=0;i<datastructs.size();++i){
    IAccDataStruct::IntersectionData idata = datastructs[i]->findClosestIntersection(ray);
    if(!idata.missed()) {
      float d;
      if(closest.missed() || closestd>(d=length(ray.getPosition() - idata.interPoint))) {
        float closestd = d;
        closest = idata;
      }
    }
  }
  return closest;
}

void CombinedDataStruct::setData(Triangle** triangles,size_t size,AABB aabb){
  for(int i=0;i<datastructs.size();++i){
    datastructs[i]->setData(triangles, size, aabb);
  }
}

} /* namespace raytracer */
