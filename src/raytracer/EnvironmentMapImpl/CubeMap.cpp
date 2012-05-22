/*
 * CubeMap.cpp
 *
 *  Created on: Apr 6, 2012
 *      Author: irri
 */

#include "CubeMap.h"
#include <iostream>

namespace raytracer {

CubeMap::CubeMap() {
  textures = NULL;
}

CubeMap::CubeMap(Texture** textures, size_t size) {
  loadTextures(textures, size);
}

CubeMap::~CubeMap() {
  if(textures!=NULL) {
    for(int i=0; i<6; ++i)
      delete textures[i];
    delete [] textures;
  }

}

void CubeMap::loadTextures(Texture** textures, size_t size) {
  if(size!=nr_of_images) {
    std::cout << "CubeMap: Error! number of images not equal to 6.\n";
    return;
  }

  this->textures = textures;
}

vec3 CubeMap::getSpecularColor(vec3 dir) {
  if(textures==NULL)
    return vec3(0,1,0);

  // 0: top      y
  // 1: bottom  -y
  // 2: front   -z
  // 3: right    x
  // 4: back     z
  // 5: left    -x

  // start values
  float max  = glm::abs(dir.y);
  int quad   = dir.y >= 0 ? 0 : 1;
  vec2 coord = vec2(dir.x, dir.z);

  if (glm::abs(dir.x) > max) {
    max = glm::abs(dir.x);
    quad = dir.x >= 0 ? 3 : 5;
    coord = vec2(dir.y, dir.z);
  }
  if (glm::abs(dir.z) > max) {
    max = glm::abs(dir.z);
    quad = dir.z >= 0 ? 4 : 2;
    coord = vec2(dir.x, dir.y);
  }

  coord /= max;
  coord = (coord + vec2(1.0f,1.0f)) / 2.0f;

  return getColorFromImage(coord, quad);
}

vec3 CubeMap::getDiffuseColor(vec3 normal) {
  return vec3();
}

vec3 CubeMap::getColorFromImage(vec2 coord, int quad) {
  return textures[quad]->getColorAt(coord);
}

} /* namespace raytracer */
