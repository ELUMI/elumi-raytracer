/*
 * ITracer.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef ITRACER_H_
#define ITRACER_H_

namespace raytracer {

class ITracer {
public:
	virtual ~ITracer();
	virtual void trace(Ray* ray,Buffer* pBuffer) = 0;
private:
	virtual void shade() = 0;
};

}

#endif /* ITRACER_H_ */
