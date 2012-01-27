
#include <vector>
#include <glm/glm.hpp>

#include "ArrayDataStruct.h"
#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"

using namespace glm;

namespace raytracer {

IAccDataStruct::~IAccDataStruct(){}

ArrayDataStruct::ArrayDataStruct(){}

ArrayDataStruct::ArrayDataStruct(int size) {
  triangles.reserve(size);
}
ArrayDataStruct::~ArrayDataStruct() {

}

Triangle ArrayDataStruct::findClosestIntersection(Ray ray) {
  
  for(int i = 0; i < triangles.size(); i++) {
    vec3* vertices;
    vec3* v0;
  }

  return Triangle();
}

void ArrayDataStruct::addData(Triangle* new_triangles, int length) {
  for(int i = 0; i < length; i++) {
    triangles.push_back(new_triangles++);
  }
}

}
