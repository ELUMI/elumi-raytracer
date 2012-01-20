/*
 * IAccDataStruct.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef IACCDATASTRUCT_H_
#define IACCDATASTRUCT_H_

namespace raytracer {

class IAccDataStruct {
public:
	virtual ~IAccDataStruct();
	virtual Triangle* findClosestIntersection(Ray ray);
};

}

#endif /* IACCDATASTRUCT_H_ */
