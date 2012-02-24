
#include <vector>
#include <glm/glm.hpp>

#include <iostream>

#include "../IAccDataStruct.h"
#include "ArrayDataStruct.h"
#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"

using namespace glm;

namespace raytracer {

IAccDataStruct::~IAccDataStruct(){}

ArrayDataStruct::ArrayDataStruct() {
  triangles.reserve(10000);
}

ArrayDataStruct::ArrayDataStruct(int size) {
  triangles.reserve(size);
}
ArrayDataStruct::~ArrayDataStruct() {

}

IAccDataStruct::IntersectionData 
ArrayDataStruct::findClosestIntersection(Ray ray) {
  vec3 o = ray.getPosition();
  vec3 d = ray.getDirection();

  Triangle* closest_tri = NULL;
  float closest_dist = -1;
  vec3 closest_pos;

  float closest_t = numeric_limits<float>::infinity( );

  for(int i = 0; i < triangles.size(); i++) {
    Triangle* cur_triangle = triangles.at(i);
    const vector<vec3*> vertices = cur_triangle->getVertices();
    vec3 v0 = *(vertices[0]);
    vec3 v1 = *(vertices[1]);
    vec3 v2 = *(vertices[2]);

    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    vec3 s = o - v0;

    vec3 dxe2 = cross(d, e2);
    vec3 sxe1 = cross(s, e1);
    vec3 res = ( 1.0f /  dot(dxe2, e1) ) * 
      vec3( dot(sxe1, e2), dot(dxe2, s), dot(sxe1, d) );

    float t = res.x;
    float u = res.y;
    float v = res.z;

    float dist = glm::distance(o, closest_pos);

    if(u >= 0 && v >= 0 && u + v <= 1
        ) {  // Intersection!
      if(t > 0 && t < closest_t) {
        closest_tri = cur_triangle;
        closest_pos = o + t * d;
        closest_dist = dist;
        closest_t = t;
      }
    }
  }

  if(closest_t == numeric_limits<float>::infinity( )) {
    return IntersectionData(IntersectionData::NOT_FOUND, vec3(), vec3(), vec2());
  }

  vec3 v1v0 = *(closest_tri->getVertices()[1]) - *(closest_tri->getVertices()[0]);
  vec3 v2v1 = *(closest_tri->getVertices()[2]) - *(closest_tri->getVertices()[1]);
  vec3 v2v0 = *(closest_tri->getVertices()[2]) - *(closest_tri->getVertices()[0]);
  vec3 pv0 = closest_pos - *(closest_tri->getVertices()[0]);
  vec3 pv1 = closest_pos - *(closest_tri->getVertices()[1]);

  float a = length(cross(v1v0, v2v0));
  float a0 = length(cross(v2v1, pv1))/a;
  float a1 = length(cross(v2v0, pv0))/a;
  float a2 = length(cross(v1v0, pv0))/a;

  vec3 inter_normal = a0 * *(closest_tri->getNormals()[0]) +
                      a1 * *(closest_tri->getNormals()[1]) +
                      a2 * *(closest_tri->getNormals()[2]);

  vec3 inter_tex =    a0 * *(closest_tri->getTextures()[0]) +
                      a1 * *(closest_tri->getTextures()[1]) +
                      a2 * *(closest_tri->getTextures()[2]);

  return IntersectionData(closest_tri->getMaterial(), closest_pos, inter_normal, vec2(inter_tex));
}

void ArrayDataStruct::setData(std::vector<Triangle*> new_triangles) {
  triangles = new_triangles;
}

}
