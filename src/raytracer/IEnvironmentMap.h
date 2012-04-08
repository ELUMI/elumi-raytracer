/*
 * IEnvironmentMap.h
 *
 *  Created on: Apr 6, 2012
 *      Author: irri
 */

#ifndef IENVIRONMENTMAP_H_
#define IENVIRONMENTMAP_H_

#include <glm/glm.hpp>

#include "utilities/Ray.h"
#include "scene/Texture.h"

namespace raytracer {

class IEnvironmentMap {
public:
  virtual ~IEnvironmentMap() {};

  virtual void loadTextures(Texture** textures, size_t size) = 0;
  virtual vec3 getColor(Ray ray) = 0;

};

} /* namespace raytracer */
#endif /* IENVIRONMENTMAP_H_ */
