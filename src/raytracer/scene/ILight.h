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

class ILight : public IDraw {
public:

  enum FalloffType { NONE, LINEAR, QUADRATIC, FACTOR };

  virtual ~ILight() {};
  virtual vec3 getPosition() = 0;
  virtual float getIntensity(float distance) = 0; //TODO make this protected
  virtual vec3 getColor() const = 0;
  virtual FalloffType getFalloffType() const {return NONE;}
  virtual float getPower() { vec3 c = getColor(); return (c.r+c.g+c.b)/3; }

  virtual void setPosition(vec3 position) = 0;
  virtual void setIntensity(float intensity) = 0;
  virtual void setColor(vec3 color) = 0;
  virtual void setDistanceFalloff(FalloffType falloff_type) = 0;

  virtual void draw() = 0;

  virtual float calcLight(IAccDataStruct* datastruct, vec3 point, vec3 offset = vec3(0.0f,0.0f,0.0f)) = 0;
  virtual void getRays(Ray* rays, size_t n) = 0;
};

}

#endif /* ILIGHT_H_ */
