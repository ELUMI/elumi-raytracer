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

  void trace(Ray* rays, int length, unsigned char* buffer);
private:
  void shade();

  const Scene* scene;
};

}

#endif /* SIMPLETRACER_H_ */
