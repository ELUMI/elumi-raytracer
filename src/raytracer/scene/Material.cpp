/*
 * Material.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Material.h"

namespace raytracer {

Material::Material() {
	// Default grey color
	this->color.r = 100;
	this->color.g = 100;
	this->color.b = 100;
	this->color.a = 255;
}

Material::~Material() {

}

void Material::setColor(vec4 color) {
  Material::color = color;
}

vec4 Material::getColor() {
  return color;
}

}
