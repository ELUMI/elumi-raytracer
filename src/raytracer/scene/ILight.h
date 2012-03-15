/*
 * ILight.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef ILIGHT_H_
#define ILIGHT_H_

#include <glm/glm.hpp>
#include <GL/glfw.h>

#include "../IDraw.h"
#include "../IAccDataStruct.h"

using namespace glm;

namespace raytracer {

enum FalloffType { NONE, LINEAR, QUADRATIC, FACTOR };

class ILight {
public:
  virtual ~ILight() {};
  virtual vec3 getPosition() = 0;
  virtual float getIntensity(float distance) = 0;
  virtual vec3 getColor() const = 0;

  virtual void setIntensity(float intensity) = 0;
  virtual void setColor(vec3 color) = 0;
  virtual void setDistanceFalloff(FalloffType falloff_type) = 0;

  virtual float distanceToBlocker(IAccDataStruct* datastruct, vec3 point) = 0;
  virtual bool isBlocked(IAccDataStruct* datastruct, vec3 point) {
    return distanceToBlocker(datastruct, point) > 0.0001f;
  }
};

}

#endif /* ILIGHT_H_ */
