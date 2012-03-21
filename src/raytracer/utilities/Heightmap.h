/*
 * Heightmap.h
 *
 *  Created on: Mar 20, 2012
 *      Author: david
 */

#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_

#include "Triangle.h"
#include "Ray.h"
#include <cstring>
#include <glm/glm.hpp>

namespace raytracer {

class Heightmap {
public:
  Heightmap(size_t width, size_t height, float real_width,
      float real_height, vec3 position, unsigned char* data);
  virtual ~Heightmap();

  Triangle getTriangle(Ray ray, vec3 intersection_point);

  unsigned char* data;
  unsigned int max_elevation;
  size_t width,height;
  float real_height,real_width;
  vec3 position;
};

}

#endif /* HEIGHTMAP_H_ */
