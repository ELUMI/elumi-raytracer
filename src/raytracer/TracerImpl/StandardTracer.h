/*
 * StandardTracer.h
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */

#ifndef STANDARDTRACER_H_
#define STANDARDTRACER_H_

#include "BaseTracer.h"

namespace raytracer {

class StandardTracer: public BaseTracer {
public:
  StandardTracer(Scene* scene);
  virtual ~StandardTracer();

  virtual void traceImage(float* color_buffer);

protected:
  virtual vec4 trace(Ray ray, IAccDataStruct::IntersectionData idata);
  virtual vec4 tracePrim(Ray ray, float attenuation, unsigned short depth);
  virtual vec4 shade(Ray ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth);
  virtual void initTracing();


  const unsigned short MAX_RECURSION_DEPTH;
  const float ATTENUATION_THRESHOLD;

  vec3 bumpMap(Material* material, IAccDataStruct::IntersectionData& idata, Ray incoming_ray);
  vec3 brdf(vec3 incoming_direction, vec3 outgoing_direction, vec3 normal, Material * material, vec3 texture_color = vec3(1,1,1));
  vec3 getTextureColor(Material* material, IAccDataStruct::IntersectionData & idata);
  vec3 reflection_refraction(float attenuation, unsigned short  depth, Material *material, vec3 normal, Ray incoming_ray, IAccDataStruct::IntersectionData idata, vec3 color);
};

}

#endif /* STANDARDTRACER_H_ */
