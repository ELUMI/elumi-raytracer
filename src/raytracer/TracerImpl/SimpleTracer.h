/*
 * SimpleTracer.h
 *
 *  Created on: Jan 26, 2012
 *      Author: david
 */

#ifndef SIMPLETRACER_H_
#define SIMPLETRACER_H_

#include "../ITracer.h"
#include "../IAccDataStruct.h"
#include "../scene/Scene.h"

namespace raytracer {

class SimpleTracer : public ITracer {
public:
  SimpleTracer(Scene* scene);
  virtual ~SimpleTracer();

  void trace(Ray* rays, int length, uint8_t* buffer);

private:
  Color traceHelper(Ray* ray, int levels=3);
  Scene* scene;
  uint8_t* buffer;
};

}

#endif /* SIMPLETRACER_H_ */
