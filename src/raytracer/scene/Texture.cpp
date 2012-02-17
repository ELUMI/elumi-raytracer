/*
 * Texture.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: DmZ
 */



#include "Texture.h"

namespace raytracer {

Texture::Texture() {
	// TODO Auto-generated constructor stub

}

Texture::Texture(unsigned int width, unsigned int height, unsigned char* data) {
  Texture::width = width;
  Texture::height = height;
  Texture::data = data;
}

Texture::~Texture() {
	// TODO Auto-generated destructor stub
}

unsigned int Texture::getHeight() {
  return height;
}

unsigned int Texture::getWidth() {
  return width;
}

vec3 Texture::getColorAt(int x, int y) {
  return vec3(data[width*y*3+x*3],data[width*y*3+x*3+1],data[width*y*3+x*3+2]);
}

}


