/*
 * ITracer.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david2
 */

#ifndef ITRACER_H_
#define ITRACER_H_

#include "utilities/Ray.h"

namespace raytracer {

class ITracer {
public:
	virtual ~ITracer() {};
	virtual void trace(Ray* ray, unsigned char* buffer) = 0;
private:
	virtual void shade() = 0;
};

}

#endif /* ITRACER_H_ */
