/*
 * HashDataStruct.cpp
 *
 *  Created on: Mar 23, 2012
 *      Author: ulvinge
 */

#include "HashDataStruct.h"
#include "HashDataStruct.h"

#include "ArrayDataStruct.h"

#include <vector>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;


namespace raytracer {

HashDataStruct::HashDataStruct(float bucketsize, size_t n_buckets)
  : hashpoint(bucketsize, n_buckets) {
}

HashDataStruct::~HashDataStruct() {
}


void getMaxMin(Triangle* t, vec3& min, vec3& max){
  vector<vec3*> vertices = t->getVertices();
  min= *(vertices[0]);
  max= *(vertices[0]);
  for(size_t j=1; j<vertices.size(); ++j){
    min = glm::min(min, *(vertices[j]));
    max = glm::max(max, *(vertices[j]));
  }
}

void HashDataStruct::setData(std::vector<Triangle*> triangles){
  for(size_t i=0; i<triangles.size(); ++i){
    Triangle* t = triangles[i];
    vec3 min,max;
    getMaxMin(t, min, max);

    //hashpoint.addGrid(min, max, t);

    float bucketsize = hashpoint.getBucketSize();

    for(float x=min.x; x<max.x+bucketsize; x+=bucketsize){
      for(float y=min.y; y<max.y+bucketsize; y+=bucketsize){
        for(float z=min.z; z<max.z+bucketsize; z+=bucketsize){
          HashedTriangle i;
          i.hash = hashpoint.hash(vec3(x,y,z));
          i.triangle = t;
          hashpoint.addItem(i.hash, i);
        }
      }
    }
  }
}

float getTMax(float p, float dir, float size){
  if(dir > 0) {
    p = (1-mod(p,size))/dir;
  } else {
    p = mod(p,size)/-dir;
  }
}

IAccDataStruct::IntersectionData HashDataStruct::findClosestIntersection(Ray ray){
  vec3 point = ray.getPosition();
  vec3 dir = ray.getDirection();
  float bucketsize = hashpoint.getBucketSize();
  //http://www.cse.yorku.ca/~amana/research/grid.pdf
  vec3 step = glm::sign(dir);
  vec3 max;
  max.x = getTMax(point.x, dir.x, bucketsize);
  max.y = getTMax(point.y, dir.y, bucketsize);
  max.z = getTMax(point.z, dir.z, bucketsize);

  vec3 delta = vec3(bucketsize/dir.x, bucketsize/dir.y, bucketsize/dir.z);
  ArrayDataStruct ads;

  size_t length = size_t(10.0f/bucketsize); //some hueristic

  while(--length) {
    size_t hash = hashpoint.hash(point);
    vector<HashedTriangle> list = hashpoint.getBucket(hash);
    vector<Triangle*> goodlist;
    for(size_t i=0; i<list.size(); ++i){
      if(list[i].hash == hash) {
        //goodlist.push_back(list[i].triangle);
      }
      goodlist.push_back(list[i].triangle);
    }

    if(goodlist.size()){
      ads.setData(goodlist);
      IAccDataStruct::IntersectionData idata = ads.findClosestIntersection(Ray(point,dir));
      if(idata.material != IAccDataStruct::IntersectionData::NOT_FOUND){
        return idata;
      }
    }

    if (max.x < max.y) {
      if (max.x < max.z) {
        point.x += step.x;
        max.x  += delta.x;
      } else {
        point.z += step.z;
        max.z  += delta.z;
      }
    } else {
      if (max.y < max.z) {
        point.y += step.y;
        max.y  += delta.y;
      } else {
        point.z += step.z;
        max.z  += delta.z;
      }
    }
  }

  //not found
  return IntersectionData(IntersectionData::NOT_FOUND, vec3(), vec3(), vec2(),vec3(),vec3());
}


}
