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

namespace raytracer {

class IAccDataStruct {
public:
  struct IntersectionData {
    IntersectionData(Triangle* triangle, vec3 interPoint, vec3 normal) {
      IntersectionData::triangle = triangle;
      IntersectionData::interPoint = interPoint;
      IntersectionData::normal = normal;
    };

    Triangle* triangle;
    vec3 interPoint;
    vec3 normal;
  };

  class NoIntersectionException : public std::runtime_error {
  public:
    NoIntersectionException() : std::runtime_error("No intersection!") {};
  };

	virtual ~IAccDataStruct()= 0;
	virtual IntersectionData findClosestIntersection(Ray ray)=0;
	virtual void addData(Triangle* triangles, int length) = 0;
};

}

#endif /* IACCDATASTRUCT_H_ */
