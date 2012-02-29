
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
  void setData(std::vector<Triangle*> new_triangles,AABB* aabb);


private:
  std::vector<Triangle*> triangles;
  AABB* aabb;
};

}

#endif
