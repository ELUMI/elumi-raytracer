/*
 * IAccDataStruct.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef IACCDATASTRUCT_H_
#define IACCDATASTRUCT_H_

#include "utilities/Triangle.h"
#include "utilities/Ray.h"

namespace raytracer {

class IAccDataStruct {
public:
  struct IntersectionData {
    Triangle* triangle;
    vec3 interPoint;
    vec3 normal;
  };

	virtual ~IAccDataStruct()= 0;
	virtual Triangle findClosestIntersection(Ray ray)=0;
	virtual void addData(Triangle* triangles, int length) = 0;
};

}

#endif /* IACCDATASTRUCT_H_ */
