
#include <vector>
#include <glm/glm.h>

#include "ArrayDataStruct.h"
#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"

namespace raytracer {

ArrayDataStruct::ArrayDataStruct() {
  triangles.reserve(10000);
}
  
ArrayDataStruct::~ArrayDataStruct() {

}

Triangle ArrayDataStruct::findClosestIntersection(Ray ray) {
  return Triangle();
}

void ArrayDataStruct::addData(Triangle* new_triangles, int length) {
  for(int i = 0; i < length; i++) {
    triangles.push_back(new_triangles++);
  }
}

}
