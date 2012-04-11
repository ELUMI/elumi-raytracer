/*
 * ITracer.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david2
 */

#ifndef ITRACER_H_
#define ITRACER_H_

namespace raytracer {

class ITracer {
public:
	virtual ~ITracer() {};
	virtual void runWithGL() = 0;
	virtual void traceImage(float* color_buffer) = 0;
	virtual void stopTracing() = 0;
  virtual float getPixelsLeft() = 0;

};

}

#endif /* ITRACER_H_ */
