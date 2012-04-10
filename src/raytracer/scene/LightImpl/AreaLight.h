/*
 * AreaLight.h
 *
 *  Created on: Mar 19, 2012
 *      Author: irri
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "../ILight.h"
#include "BaseLight.h"
#include "../Scene.h"

namespace raytracer {

class AreaLight: public BaseLight {
public:
  AreaLight();
  AreaLight(vec3 position, vec3 axis1, vec3 axis2, unsigned int sample1, unsigned int sample2);
  virtual ~AreaLight();

  float getFalloff(float distance);
  FalloffType getFalloffType() const;

  void setPosition(vec3 position);
  void setIntensity(float intensity);
  void setColor(vec3 color);
  void setDistanceFalloff(FalloffType falloff_type);

  void getRays(Ray* rays, size_t n, int thread_id);

  void draw();
  void addPlane(Scene* scene);

  void initCaches(size_t nr_of_threads);

  float calcLight(IAccDataStruct* datastruct, vec3 point, int thread_id, vec3 offset = vec3(0.0f,0.0f,0.0f));

private:
  vec3 axis1, axis2;
  vec3 delta1, delta2;
  unsigned int sample1, sample2;
  unsigned int samples;


  BaseLight* light_sources;
};

}

#endif /* AREALIGHT_H_ */
