/*
 * AreaLight.h
 *
 *  Created on: Mar 19, 2012
 *      Author: irri
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "../ILight.h"
#include "OmniLight.h"
#include "../../utilities/Random.h"

namespace raytracer {

class AreaLight: public ILight {
public:
  AreaLight();
  AreaLight(vec3 position, vec3 axis1, vec3 axis2, unsigned int sample1, unsigned int sample2);
  virtual ~AreaLight();

  vec3 getPosition();
  float getIntensity(float distance);
  vec3 getColor() const;
  FalloffType getFalloffType() const;

  void setPosition(vec3 position);
  void setIntensity(float intensity);
  void setColor(vec3 color);
  void setDistanceFalloff(FalloffType falloff_type);

  void getRays(Ray* rays, size_t n);


  virtual void draw();

  float calcLight(IAccDataStruct* datastruct, vec3 point, vec3 offset = vec3(0.0f,0.0f,0.0f));

private:
  vec3 position;
  vec3 axis1, axis2;
  vec3 delta1, delta2;
  unsigned int sample1, sample2;
  unsigned int samples;

  float intensity;
  vec3 color;
  FalloffType falloff_type;

  OmniLight* light_sources;
  Random rand;
};

}

#endif /* AREALIGHT_H_ */
