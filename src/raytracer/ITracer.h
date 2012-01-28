/*
 * ITracer.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david2
 */

#ifndef ITRACER_H_
#define ITRACER_H_

#include "scene/Scene.h"
#include "utilities/Ray.h"

namespace raytracer {

class ITracer {
public:
  /*ITracer();
  ITracer(Scene* scene);*/
	virtual ~ITracer() {};
	virtual void trace(Ray* rays, int length/*, unsigned char* buffer*/) = 0;
private:
	virtual Color shade(Ray* ray, Material* material,) = 0;

};

}

#endif /* ITRACER_H_ */
