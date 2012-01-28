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
    Triangle* triangle;
    vec3 interPoint;
    vec3 normal;
  };

//  IAccDataStruct();
	virtual ~IAccDataStruct()= 0;
	virtual IntersectionData findClosestIntersection(Ray ray) =0;
	virtual void addData(Triangle* triangles, int length) = 0;
};

}

#endif /* IACCDATASTRUCT_H_ */
