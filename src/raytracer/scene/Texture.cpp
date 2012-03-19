/*
 * Texture.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: DmZ
 */



#include "Texture.h"

#include <iostream>
using namespace std;

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
  vec3 e1 = v1v0;
  vec3 n = cross(v1v0,v2v0);
  vec3 e2 = cross(n,e1);
  e2 = normalize(e2);

  coords.x = length(dot(position,normalize(e1))*normalize(e1))/v1v0.length();
  coords.y = length(dot(position,normalize(e2))*normalize(e2))/e2.length();
  return coords;
}

//Get UV-coordinates by dropping one axis
vec2 Texture::getUVCoordinates(vec3 position, Axis axis) {
  switch (axis) {
    case XAXIS:
      return vec2(position.y,position.z);
      break;
    case YAXIS:
      return vec2(position.x,position.z);
      break;
    case ZAXIS:
      return vec2(position.x,position.y);
      break;
    default:
      return vec2(0,0);
      break;
  }
}

vec3 Texture::getColorAt(vec2 coords) {
  int x_coord,y_coord;
  if(coords.x < 0) {
    x_coord = width+(int)(coords.x*(width))%width;
  } else {
    x_coord = (int)(coords.x*(width))%width;
  }
  if(coords.y < 0) {
    y_coord = height+(int)(coords.y*(height))%height;
  } else {
    y_coord = (int)(coords.y*(height))%height;
  }
  //x_coord = (int)(coords.x*(width))%width;
  //y_coord = (int)(coords.y*(height))%height;
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


