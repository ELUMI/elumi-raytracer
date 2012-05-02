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
#include "../IDeferredProcesser.h"
#include "../IRenderPattern.h"

#include <boost/thread/mutex.hpp>

namespace raytracer {

inline float clamp(float x)
{
    return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);
}

class BaseTracer: public raytracer::ITracer {
public:
  BaseTracer(Scene* scene);

  virtual ~BaseTracer();

  virtual void traceImage(float* color_buffer);
  void runWithGL();
  virtual void first_bounce();

  void         stopTracing();
  float getPixelsLeft();
  virtual void initTracing();

  vec3* posbuff;
protected:
  virtual vec4 trace(Ray ray, IAccDataStruct::IntersectionData idata, int thread_id);
  virtual vec4 shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata, int thread_id);
  virtual void initThreads(int nr_threads);
  size_t* position;

  Scene* scene;
  Settings* settings;
  float* buffer;

  IAccDataStruct* datastruct;
  std::vector<ILight*>* lights;

  bool abort;

  IDeferredProcesser* first_pass;
  IAccDataStruct::IntersectionData* first_intersections;

private:
  void traceImageThread(int id);

  IRenderPattern* pattern;
  int nr_batches;
  int next_batch;
  boost::mutex pattern_mutex;
};

}

#endif /* BASETRACER_H_ */
