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
  SimpleTracer(Scene* scene, vec4 background_color = vec4(0,0,0,255));
  virtual ~SimpleTracer();

  void trace(Ray* rays, int length, uint8_t* buffer);

private:
  vec4 traceHelper(Ray* ray, int levels=3);
  Scene* scene;
  vec4 background_color;
  uint8_t* buffer;
};

}

#endif /* SIMPLETRACER_H_ */
