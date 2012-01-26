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
  virtual ~ArrayDataStruct();

  Triangle findClosestIntersection(Ray ray);
  void addData(Triangle* new_triangles, int length);

private:
  std::vector<Triangle*> triangles;
};

}

#endif
