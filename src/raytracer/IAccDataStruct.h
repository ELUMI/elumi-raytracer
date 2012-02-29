/*
 * IAccDataStruct.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef IACCDATASTRUCT_H_
#define IACCDATASTRUCT_H_

#include <stdexcept>

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

    vector<vec3*> uvcoords;
    unsigned int material,texture;
    vec3 interPoint;
    vec3 normal;
    vec2 texcoord;
    vec3 e1,e2;
    const static int NOT_FOUND = -1;
  };


	virtual ~IAccDataStruct()= 0;
	virtual IntersectionData findClosestIntersection(Ray ray)=0;
	virtual void setData(std::vector<Triangle*> triangles) = 0;

};

}

#endif /* IACCDATASTRUCT_H_ */
