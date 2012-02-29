/*
 * Texture.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: DmZ
 */



#include "Texture.h"

namespace raytracer {

Texture::Texture() {
}

Texture::Texture(unsigned int width, unsigned int height, unsigned char* data, TextureType type) {
  Texture::width = width;
  Texture::height = height;
  Texture::data = data;
  Texture::type = type;
}

Texture::Texture(unsigned int width, unsigned int height, unsigned char* data) {
  Texture::width = width;
  Texture::height = height;
  Texture::data = data;
  Texture::type = TEXTURE;
}

Texture::~Texture() {
}

unsigned int Texture::getHeight() {
  return height;
}

unsigned int Texture::getWidth() {
  return width;
}

vec3 Texture::getColorAt(int x, int y) {
  return vec3(data[width*y*3+x*3],data[width*y*3+x*3+1],data[width*y*3+x*3+2])/255.0f;
}

vec2 Texture::getUVCoordinates(vec3 position, MappingType type) {
  vec2 coords = vec2(0,0);
  switch (type) {
    case PLANAR:
      float x,y;
      x = abs(position.x);
      y = abs(position.y);
      if(x > y) {
        x = abs(position.y);
        y = abs(position.x);
      }
      if(y > abs(position.z)) {
        y = abs(position.z);
      }
      coords = vec2(x,y);
      break;
    default:
      break;
  }
  return coords;
}

vec3 Texture::getColorAt(vec2 coords) {
  int x_coord,y_coord;
  x_coord = (int)(coords.x*(width))%width;
  y_coord = (int)(coords.y*(height))%height;
  return getColorAt(x_coord,y_coord);
}

}


