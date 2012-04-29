/*
 * PointLight.h
 *
 *  Created on: 18 apr 2012
 *      Author: julian
 */

#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include "BaseLight.h"

#include "glm/glm.hpp"

// gl.h och glew.h includeras i fel ordning om jag inkluderar glutil.h här.
// därför så kopierade jag glutils definition av M_PI
#ifndef M_PI
# define M_PI 3.14159265358979323846f
#endif

namespace raytracer {

class SpotLight: public BaseLight {
public:
  // Degrees
  SpotLight(vec3 pos = vec3(), vec3 dir = vec3(), float inner = 30.f, float outer = 30.f) : BaseLight(pos) {
    SpotLight::dir = glm::normalize(dir);
    SpotLight::inner = inner;
    SpotLight::outer = outer;
  }
  virtual ~SpotLight();

  virtual void getRays(Ray* rays, size_t n, int thread_id);

  float calcLight(IAccDataStruct* datastruct, vec3 point, int thread_id, vec3 offset = vec3(0.0f,0.0f,0.0f));


protected:
  vec3 dir;
  float inner;
  float outer;
};

} /* namespace raytracer */
#endif /* SPOTLIGHT_H_ */
