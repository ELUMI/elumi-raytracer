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

//  width = height = 50;
//
//  image_data = new vec4**[nr_of_images];
//  for(size_t i=0; i<nr_of_images; ++i) {
//    vec4** rows = new vec4*[height];
//    for(size_t h=0; h<height; ++h) {
//      vec4* line = new vec4[width];
//      for(size_t w=0; w<width; ++w) {
//        if(w%10==0 && h%10==0)
//          line[w] = vec4(0,1,1,1);
//        else
//          line[w] = vec4(0,1,0,1);
//      }
//      rows[h] = line;
//    }
//    image_data[i] = rows;
//  }

}

vec3 CubeMap::getColor(Ray ray) {
  if(textures==NULL)
    return vec3(0,1,0);

  vec3 dir = ray.getDirection();

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

  //std::cout << "quad: " << quad << ", coord: " << coord.x << ", " << coord.y << "\n";

  return getColorFromImage(coord, quad);
}

vec3 CubeMap::getColorFromImage(vec2 coord, int quad) {
  return textures[quad]->getColorAt(coord);
}

} /* namespace raytracer */
