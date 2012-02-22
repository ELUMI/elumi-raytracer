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

Texture::Texture(unsigned int width, unsigned int height, unsigned char* data) {
  Texture::width = width;
  Texture::height = height;
  Texture::data = data;
}

Texture::~Texture() {
}

}


