/*
 * OmniLight.h
 *
 *  Created on: Feb 1, 2012
 *      Author: david
 */

#ifndef OMNILIGHT_H_
#define OMNILIGHT_H_

#include <glm/glm.hpp>
#include "../ILight.h"

using namespace glm;

namespace raytracer {

class OmniLight : public ILight {
public:
  OmniLight();
  OmniLight(vec3 position);
  virtual ~OmniLight();

  vec3 getPosition();
  float getIntensity(float distance);
  vec3 getColor() const;
  FalloffType getFalloffType() const;

  void setPosition(vec3 position);
  void setDistanceFalloff(FalloffType falloff_type);
  void setIntensity(float intesity);
  void setColor(vec3 color);

  void getRays(Ray* rays, size_t n);

  void draw();

  float calcLight(IAccDataStruct* datastruct, vec3 point, vec3 offset = vec3(0.0f,0.0f,0.0f));

private:
  float distanceToBlocker(IAccDataStruct* datastruct, vec3 point, vec3 offset = vec3(0.0f,0.0f,0.0f));
  bool isBlocked(IAccDataStruct* datastruct, vec3 point, vec3 offset = vec3(0.0f,0.0f,0.0f)) {
    return distanceToBlocker(datastruct, point, offset) > 0.0001f;
  }

  vec3 m_position;
  float m_intensity;
  vec3 color;
  FalloffType m_falloff_type;
};

}

#endif /* OMNILIGHT_H_ */
