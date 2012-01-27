
#include <vector>
#include <glm/glm.hpp>

#include "../IAccDataStruct.h"
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

IAccDataStruct::IntersectionData ArrayDataStruct::findClosestIntersection(Ray ray) {
  vec3 o = ray.getPos();
  vec3 d = ray.getDir();

  for(int i = 0; i < triangles.size(); i++) {
    vec3* vertices = triangles.at(i)->getVertices();
    vec3 v0 = *vertices;
    vec3 v1 = *(vertices + 1);
    vec3 v2 = *(vertices + 2);

    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    vec3 s = o - v0;

    vec3 dxe2 = d * e2;
    vec3 sxe1 = s * e1;
    vec3 res = ( 1.0f /  dot(dxe2, e1) ) * 
      vec3( dot(sxe1, e2), dot(dxe2, s), dot(sxe1, d) );

    float t = res.x;
    float u = res.y;
    float v = res.z;
  }

  return IntersectionData();
}

void ArrayDataStruct::addData(Triangle* new_triangles, int length) {
  for(int i = 0; i < length; i++) {
    triangles.push_back(new_triangles++);
  }
}

}
