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
    IntersectionData(unsigned int material, vec3 interPoint, vec3 normal, vec2 texcoord,vec3 e1, vec3 e2) {
      IntersectionData::material = material;
      IntersectionData::texture = texture;
      IntersectionData::interPoint = interPoint;
      IntersectionData::normal = normal;
      IntersectionData::texcoord = texcoord;
      IntersectionData::e1 = e1;
      IntersectionData::e2 = e2;
    };

    unsigned int material,texture;
    vec3 interPoint;
    vec3 normal;
    vec2 texcoord;
    vec3 e1,e2;
    const static unsigned int NOT_FOUND_INTERNAL = -1;

    bool missed() { return material == NOT_FOUND_INTERNAL; }
    static IntersectionData miss() { return IntersectionData(NOT_FOUND_INTERNAL, vec3(), vec3(), vec2(),vec3(),vec3()); }
  };


	virtual ~IAccDataStruct() {};
	virtual IntersectionData findClosestIntersection(Ray ray)=0;
	virtual void setData(Triangle** triangles,size_t size,AABB* aabb) = 0;
	virtual vector<AABB*>& getAABBList() = 0;

};

}

#endif /* IACCDATASTRUCT_H_ */
