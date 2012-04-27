/*
 * Texture.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: DmZ
 */



#include "Texture.h"

#include <iostream>
#include "../utilities/glutil.h"
using namespace std;

namespace raytracer {

Texture::Texture() {
  Texture::width = -1;
  Texture::height = -1;
  Texture::data = NULL;
  Texture::type = TEXTURE;
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

void Texture::setData(unsigned int width, unsigned int height, unsigned char* data,TextureType type) {
  Texture::width = width;
  Texture::height = height;
  Texture::data = data;
  Texture::type = type;
}

Texture::~Texture() {
  // TODO ILShutdown anropas i SCENES's destructor. Den verkar ta bort datan?
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
  if (data==NULL) {
    std::cout << "Texture data not loaded!\n";
    exit(1);
  }
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

vec2 Texture::getUVCoordinates(vec3 position, vec3 normal, double scale, Projector projector, Axis axis) {
  switch (projector) {
    case PLANE:
      return planeMapping(position, axis);
      break;
    case CUBE:
      return cubeMapping(position, normal);
      break;
    case SPHERE:
      return sphereMapping(normal,axis);
      break;
    case CYLINDER:
      return cylinderMapping(normal,position, axis);
      break;
    default:
      return cubeMapping(position, normal);
      break;
  }
}

//Get UV-coordinates by dropping one axis
vec2 Texture::planeMapping(vec3 position, Axis axis) {
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

vec2 Texture::cubeMapping(vec3 position, vec3 normal) {
  float x = abs(normal.x);
  float y = abs(normal.y);
  float z = abs(normal.z);

  if(x >= glm::max(y,z)) {
    return vec2(position.y,position.z);
  } else if(y >= glm::max(x,z)) {
    return vec2(position.x,position.z);
  } else {
    return vec2(position.x,position.y);
  }
}

vec2 Texture::sphereMapping(vec3 normal, Axis axis) {

  normal = normalize(normal);

  switch (axis) {
    case XAXIS:
      return vec2(glm::asin(normal.y)/M_PI+0.5f,glm::asin(normal.z)/M_PI+0.5f);
      break;
    case YAXIS:
      return vec2(glm::asin(normal.x)/M_PI+0.5f,glm::asin(normal.z)/M_PI+0.5f);
      break;
    case ZAXIS:
      return vec2(glm::asin(normal.x)/M_PI+0.5f,glm::asin(normal.y)/M_PI+0.5f);
      break;
    default:
      return vec2(glm::asin(normal.x)/M_PI+0.5f,glm::asin(normal.y)/M_PI+0.5f);
      break;
  }
}

vec2 Texture::cylinderMapping(vec3 normal, vec3 position, Axis axis) {
  float y = round(position.y * 100000.0f)/100000.0f;
  vec2 p;

  switch (axis) {
  case XAXIS:
    p = normalize(vec2(position.y,position.z));
    return vec2((glm::atan(p.y/p.x))/(M_PI_2),y);
    break;
  case YAXIS:
    p = normalize(vec2(position.x,position.z));
    return vec2((glm::atan(p.y/p.x))/(M_PI_2),y);
    break;
  case ZAXIS:
    p = normalize(vec2(position.x,position.y));
    return vec2((glm::atan(p.y/p.x))/(M_PI_2),y);
    break;
  default:
    p = normalize(vec2(position.x,position.z));
    return vec2((glm::atan(p.y/p.x))/(M_PI_2),y);
    break;
  }
}

vec3 Texture::getColorAt(vec2 coords) {
  //return repeatImage(coords);
  return getColorAt(width*coords.u,height*coords.v);
}

vec3 Texture::repeatImage(vec2 coords) {
  int x = ((int)((coords.x-floor(coords.x))*width))%width;
  int y = ((int)((coords.y-floor(coords.y))*height))%height;
  return getColorAt(x,y);
}

vec3 Texture::clampImage(vec2 coords) {
  float u = coords.x;
  u = glm::clamp(u,0.0f,1.0f);
  float v = coords.y;
  v = glm::clamp(v,0.0f,1.0f);
  return repeatImage(vec2(u,v));
}

vec3 Texture::mirrorImage(vec2 coords) {
  float x = coords.x;
  float y = coords.y;
  float sign_x,sign_y;

  sign_x = glm::pow(-1.0f,floor(x));
  sign_y = glm::pow(-1.0f,floor(y));

  return repeatImage(vec2(x*sign_x,y*sign_y));

}

vec3 Texture::getColorAt(vec2 coords, vec3 border_color, float scale) {
  float half_scale = scale/2.0f;
  if(coords.x >= -half_scale || coords.x < half_scale ||
      coords.y >= -half_scale || coords.y < half_scale) {
    return border_color;
  } else {
    return repeatImage(coords/scale);
  }
}

vec3 Texture::getColorAt(vec2 coords, Corresponder c) {
  switch (c) {
    case REPEAT:
      return repeatImage(coords);
      break;
    case MIRROR:
      return mirrorImage(coords);
      break;
    case CLAMP:
      return clampImage(coords);
      break;
    default:
      break;
  }
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

