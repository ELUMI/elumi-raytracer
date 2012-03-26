/*
 * Heightmap.h
 *
 *  Created on: Mar 20, 2012
 *      Author: david
 */

#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_

#include <cstring>
#include <glm/glm.hpp>
#include "../utilities/Ray.h"
#include "../utilities/Triangle.h"
#include "../IAccDataStruct.h"

namespace raytracer {

class Heightmap {
public:
  Heightmap(size_t width, size_t height, float real_width,
      float real_height, vec3 position, unsigned char* data);
  virtual ~Heightmap();

  IAccDataStruct::IntersectionData getTriangle(Ray ray);

  unsigned char* data;
  unsigned int max_elevation;
  size_t width,height;
  float real_height,real_width;
  vec3 position;
};

}

#endif /* HEIGHTMAP_H_ */
