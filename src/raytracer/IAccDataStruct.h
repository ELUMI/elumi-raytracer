/*
 * IAccDataStruct.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef IACCDATASTRUCT_H_
#define IACCDATASTRUCT_H_

#include <stdexcept>

#include "common.hpp"
#include "utilities/Triangle.h"
#include "utilities/Ray.h"
using std::runtime_error;

namespace raytracer {

class NoIntersectionException : public runtime_error {
public:
  NoIntersectionException() : runtime_error("No intersection occured.") {}
};

class IAccDataStruct {
public:
  struct IntersectionData {
    IntersectionData() {}
    IntersectionData(Triangle* triangle, unsigned int material, vec3 interPoint, vec3 normal, vec2 texcoord,vec3 e1, vec3 e2) {

      IntersectionData::triangle = triangle;
      IntersectionData::material = material;
      IntersectionData::texture = texture;
      IntersectionData::interPoint = interPoint;
      IntersectionData::normal = normal;
      IntersectionData::texcoord = texcoord;
      IntersectionData::e1 = e1;
      IntersectionData::e2 = e2;
    };

    Triangle* triangle;
    unsigned int material,texture;
    vec3 interPoint;
    vec3 normal;
    vec2 texcoord;
    vec3 e1,e2;
    const static unsigned int NOT_FOUND_INTERNAL = -1;

    bool missed() { return material == NOT_FOUND_INTERNAL; }
    static IntersectionData miss() { return IntersectionData(NULL, NOT_FOUND_INTERNAL, vec3(), vec3(), vec2(),vec3(),vec3()); }
  };


  virtual ~IAccDataStruct() {};
  virtual IntersectionData findClosestIntersection(Ray ray)=0;
  virtual void setData(Triangle** triangles,size_t size,AABB aabb) = 0;
  virtual vector<AABB*>& getAABBList() = 0;

  static float instersection_distance(Ray* ray, Triangle* triangle) {
    const vec3 o = ray->getPosition();
    const vec3 d = ray->getDirection();
    const vector<vec3*> vertices = triangle->getVertices();
    const vec3 v0 = *(vertices[0]);
    const vec3 v1 = *(vertices[1]);
    const vec3 v2 = *(vertices[2]);

    const vec3 e1 = v1 - v0;
    const vec3 e2 = v2 - v0;
    const vec3 s = o - v0;

    const vec3 dxe2 = cross(d, e2);
    const vec3 sxe1 = cross(s, e1);
    const vec3 res = ( 1.0f /  dot(dxe2, e1) ) *
        vec3( dot(sxe1, e2), dot(dxe2, s), dot(sxe1, d) );

    const float t = res.x;
    const float u = res.y;
    const float v = res.z;

    if(u >= 0 && v >= 0 && u + v <= 1 && t>0) {  // Intersection!
      return t;
    }
    return -INFINITY;
  }

};

}

#endif /* IACCDATASTRUCT_H_ */
