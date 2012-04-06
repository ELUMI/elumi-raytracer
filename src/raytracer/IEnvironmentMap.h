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

namespace raytracer {

class IEnvironmentMap {
public:
  virtual ~IEnvironmentMap() {};

  virtual void loadImageFiles(const char** filenames, int length) = 0;
  virtual vec4 getColor(Ray ray) = 0;

};

} /* namespace raytracer */
#endif /* IENVIRONMENTMAP_H_ */
