/*
 * PointLight.h
 *
 *  Created on: 18 apr 2012
 *      Author: julian
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "BaseLight.h"

#include "glm/glm.hpp"
#include "../../utilities/glutil.h"

namespace raytracer {

class PointLight: public BaseLight {
public:
  PointLight(vec3 pos = vec3(), vec3 dir = vec3(), float inner = M_PI / 3.0f, float outer = M_PI / 3.0f) {
    BaseLight(pos);
    PointLight::dir = dir;
    PointLight::inner = inner;
    PointLight::outer = outer;
  }
  virtual ~PointLight();

  virtual void getRays(Ray* rays, size_t n, int thread_id);

  virtual float calcLight(IAccDataStruct* datastruct, vec3 point, int thread_id, vec3 offset = vec3(0.0f,0.0f,0.0f));


protected:
  vec3 dir;
  float inner;
  float outer;
};

} /* namespace raytracer */
#endif /* POINTLIGHT_H_ */
