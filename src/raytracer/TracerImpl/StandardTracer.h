/*
 * StandardTracer.h
 *
 *  Created on: Feb 14, 2012
 *      Author: irri
 */

#ifndef STANDARDTRACER_H_
#define STANDARDTRACER_H_

#include "../IEnvironmentMap.h"
#include "BaseTracer.h"

namespace raytracer {

class StandardTracer: public BaseTracer {
public:
  StandardTracer(Scene* scene);
  virtual ~StandardTracer();
  virtual void traceImage(float* color_buffer);

protected:
  virtual vec4 trace(Ray ray, IAccDataStruct::IntersectionData idata, int thread_id);
  virtual vec4 tracePrim(Ray ray, float attenuation, unsigned short depth, int thread_id);
  virtual vec4 shade(Ray ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth, int thread_id);
  virtual void initTracing();
  virtual void first_bounce();


  const unsigned short MAX_RECURSION_DEPTH;
  const float ATTENUATION_THRESHOLD;

  vec3 getParallax(Material* material,IAccDataStruct::IntersectionData idata,vec2 tex_coords);
  vec3 getPerturbedNormal(Ray incoming_ray, IAccDataStruct::IntersectionData idata, Material* material,vec2 tex_coords);
  vec3 brdf(vec3 incoming_direction, vec3 outgoing_direction, vec3 normal, Material * material, vec3 texture_color = vec3(1,1,1),vec2 tex_coords = vec2(0,0));
  vec2 getTextureCoordinates(Material* material, IAccDataStruct::IntersectionData idata, vec3 displacement);
  vec3 getTextureColor(Material* material, IAccDataStruct::IntersectionData idata,vec2 tex_coords);
  vec3 reflection_refraction(Ray incoming_ray, IAccDataStruct::IntersectionData idata, float attenuation,
      unsigned short  depth, Material *material, vec3 normal, vec3 color, vec2 tex_coords, int thread_id);
  vec3 getLighting(Ray incoming_ray, IAccDataStruct::IntersectionData idata, vec3 normal, Material *material, vec2 tex_coords, int thread_id, vec3 parallax);
  virtual float getIndividualLight(vec3 pos, ILight* light, int thread_id);
  virtual vec3 getAmbient(Ray incoming_ray, IAccDataStruct::IntersectionData idata, int thread_id, unsigned short depth);

private:
  bool using_environment_map;
};

}

#endif /* STANDARDTRACER_H_ */
