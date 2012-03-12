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

int Texture::getMipmapLevels() {
  return mipmap_levels;
}

vec3 Texture::getColorAt(int x, int y) {
  return vec3(data[width*y*3+x*3],data[width*y*3+x*3+1],data[width*y*3+x*3+2])/255.0f;
}

vec2 Texture::getUVCoordinates(vec3 position, vec3 v1v0, vec3 v2v0) {
  vec2 coords = vec2(0,0);
  vec3 e1 = normalize(v1v0);
  vec3 n = cross(v1v0,v2v0);
  //vec3 e2 = normalize(cross(normal-position,e1));
  vec3 e2 = cross(n,e1);
  e2 = normalize(e2);
  coords.x = length(dot(position,e1)*e1);
  coords.y = length(dot(position,e2)*e2);
  return coords;
}

vec3 Texture::getColorAt(vec2 coords) {
  int x_coord,y_coord;
  if(coords.x < 0) {
    coords.x = width-(int)abs((coords.x))%width;
  }
  if(coords.y < 0) {
    coords.y = height-(int)
        (abs(coords.y))%height;
  }
  x_coord = (int)(coords.x*(width))%width;
  y_coord = (int)(coords.y*(height))%height;
  return getColorAt(x_coord,y_coord);
}

vec3 Texture::getInterpolatedColor(vec2 coords) {
  vec3 first_color = ( getColorAt(coords) + getColorAt(vec2(coords.x+1,coords.y)) )/2.0f;
  vec3 second_color = ( getColorAt(vec2(coords.x,coords.y+1)) + getColorAt(vec2(coords.x+1,coords.y+1)) )/2.0f;
  return (first_color+second_color)/2.0f;
}

void Texture::addMipmap() {
  mipmap_levels++;
}

}


