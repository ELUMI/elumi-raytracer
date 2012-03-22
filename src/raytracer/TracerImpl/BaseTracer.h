/*
 * BaseTracer.h
 *
 *  Created on: Feb 15, 2012
 *      Author: irri
 */

#ifndef BASETRACER_H_
#define BASETRACER_H_

#include "../ITracer.h"
#include "../scene/Scene.h"
#include "../Settings.h"
#include "../utilities/Ray.h"
#include "../utilities/DeferredProcesser.h"

namespace raytracer {

inline float clamp(float x)
{
    return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);
}

class BaseTracer: public raytracer::ITracer {
public:
  BaseTracer(Scene* scene);

  virtual ~BaseTracer();

  virtual void         traceImage(float* color_buffer);
  virtual int          spawnRays();
  virtual void first_bounce();

  void         stopTracing();
  unsigned int getPixelsLeft();

  vec3* posbuff;
protected:
  virtual vec4 trace(Ray ray, IAccDataStruct::IntersectionData idata);
  virtual vec4 shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata);

  Scene* scene;
  Settings* settings;
  float* buffer;
  Ray* rays;

  IAccDataStruct* datastruct;
  std::vector<ILight*>* lights;

  bool abort;
  unsigned int pixelsLeft;

  DeferredProcesser* first_pass;
  IAccDataStruct::IntersectionData* first_intersections;

  virtual void initTracing();
private:
  virtual void tracePixel(size_t i, IAccDataStruct::IntersectionData intersection_data);
};

}

#endif /* BASETRACER_H_ */
