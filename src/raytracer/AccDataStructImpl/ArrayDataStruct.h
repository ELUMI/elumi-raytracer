
#ifndef ARRAYDATASTRUCT_H_
#define ARRAYDATASTRUCT_H_

#include <vector>

#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"
#include "../IAccDataStruct.h"

namespace raytracer {

class ArrayDataStruct : public IAccDataStruct {
public:
  ArrayDataStruct();
  ArrayDataStruct(int size);
  virtual ~ArrayDataStruct();

  IAccDataStruct::IntersectionData findClosestIntersection(Ray ray);
  void setData(Triangle** triangles,size_t size,AABB* aabb);


private:
  Triangle** triangles;
  size_t size;
  AABB* aabb;
};

}

#endif
