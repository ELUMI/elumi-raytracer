/*
 * HashDataStruct.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ulvinge
 */

#ifndef HASHDATASTRUCT_H_
#define HASHDATASTRUCT_H_

#include "../IAccDataStruct.h"
#include "../utilities/HashPoint.h"

namespace raytracer {

class HashDataStruct: public IAccDataStruct {
public:
  HashDataStruct(float bucketsize, size_t n_buckets);
  virtual ~HashDataStruct();

  IntersectionData findClosestIntersection(Ray ray);
  void setData(Triangle** triangles,size_t size,AABB* aabb);
private:
  struct HashedTriangle {
    size_t hash;
    Triangle* triangle;
  };

  HashPoint<HashedTriangle> hashpoint;
};

}

#endif /* HASHDATASTRUCT_H_ */
