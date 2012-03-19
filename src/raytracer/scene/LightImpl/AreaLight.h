/*
 * AreaLight.h
 *
 *  Created on: Mar 19, 2012
 *      Author: irri
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "../ILight.h"

namespace raytracer {

class AreaLight: public raytracer::ILight {
public:
  AreaLight();
  virtual ~AreaLight();

  virtual vec3 getPosition();
  virtual float getIntensity(float distance);
  virtual vec3 getColor() const;
  virtual FalloffType getFalloffType();

  virtual void setPosition(vec3 position);
  virtual void setIntensity(float intensity);
  virtual void setColor(vec3 color);
  virtual void setDistanceFalloff(FalloffType falloff_type);

  virtual void draw();

  float calcShadow(IAccDataStruct* datastruct, vec3 point);

private:
  virtual float distanceToBlocker(IAccDataStruct* datastruct, vec3 point);

  vec3 position;
  float intensity;
  vec3 color;
  FalloffType falloff_type;
};

}

#endif /* AREALIGHT_H_ */
