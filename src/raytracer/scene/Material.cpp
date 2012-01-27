/*
 * Material.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Material.h"

namespace raytracer {

Material::Material(int id) {
	this->id = id;

	// Default grey color
	this->color.r = 100;
	this->color.g = 100;
	this->color.b = 100;
	this->color.a = 255;
}

Material::~Material() {

}

Color Material::getColor() {
  return color;
}

}
