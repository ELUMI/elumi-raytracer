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
	Material::color.r = 100;
	Material::color.g = 100;
	Material::color.b = 100;
	Material::color.a = 255;

	// Some default values
	Material::name = "default_material";
	Material::ambient = glm::vec3(0.2f,0.2f,0.2f);
	Material::diffuse = glm::vec3(0.8f,0.8f,0.8f);
	Material::specular = glm::vec3(0.2f,0.2f,0.2f);
	Material::emissive = glm::vec3(1.0f,1.0f,1.0f); //?
	Material::transperancy = glm::vec3(1.0f,1.0f,1.0f);
	Material::shininess = 0;
	Material::sharpness = 98; //glossy
	Material::index_of_reflection = 1;
	Material::reflection = 0.0f;
	Material::alpha_texture_map = "\0";
}

Material::Material(std::string name,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,
glm::vec3 emissive,glm::vec3 transperancy,float shininess,float sharpness,
float reflection ,float index_of_reflection,std::string alpha_texture_map){

	Material::name = name;
	Material::ambient = ambient;
	Material::diffuse = diffuse;
	Material::specular = specular;
	Material::emissive = emissive;
	Material::transperancy = transperancy;
	Material::shininess = shininess;
	Material::sharpness = sharpness;
	Material::index_of_reflection = index_of_reflection;
	Material::reflection = reflection;
	Material::alpha_texture_map = alpha_texture_map;
}

Material::~Material() {

}

void Material::setColor(glm::vec4 color) {
  Material::color = color;
}

glm::vec4 Material::getColor() {
  //return color;
  return glm::vec4(diffuse,transperancy.x);
}

}
