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
#include "../scene/Texture.h"
#include "../utilities/Triangle.h"
#include "../IAccDataStruct.h"
#include "../utilities/OBB.h"

namespace raytracer {

class Heightmap {
public:
  Heightmap(size_t width, size_t height, vec3 u, vec3 v,
      vec3 position, float elevation, unsigned char* data);
  virtual ~Heightmap();

  IAccDataStruct::IntersectionData getTriangle(Ray ray);

  unsigned char* data;
  size_t width,height;
  vec3 position;
private:
  IAccDataStruct::IntersectionData triangleIntersection(Ray ray, Triangle* triangle1, Triangle* triangle2);
  IAccDataStruct::IntersectionData createTrianglesAndIntersect(Ray ray, vec3 v0, vec3 v1, vec3 v2, vec3 v3);
  OBB bb;
  vec3 u,v,w;
  float elevation,max_elevation;
  Texture* texture;
};

}

#endif /* HEIGHTMAP_H_ */
