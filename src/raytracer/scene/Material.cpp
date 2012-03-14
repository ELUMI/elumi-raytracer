/*
 * Material.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Material.h"

namespace raytracer {

Material::Material() {


  // Some default values
  Material::name = "default_material";
  Material::ambient = glm::vec3(0.2f,0.2f,0.2f);
  Material::diffuse = glm::vec3(0.8f,0.8f,0.8f);
  Material::specular = glm::vec3(0.2f,0.2f,0.2f);
  Material::emissive = glm::vec3(1.0f,1.0f,1.0f); //?
  Material::opacity = 1.0f;
  Material::shininess = 0.0f;
  Material::sharpness = 98; //glossy
  Material::index_of_refraction = 1.0f;
  Material::reflection = 0.0f;
  Material::texture = -1;
  Material::bump_map = -1;
}

Material::Material(std::string name,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,
    glm::vec3 emissive,glm::vec3 _opacity,float shininess,float sharpness,
    float reflection ,float index_of_refraction, int texture, int bump_map){

  Material::name = name;
  Material::ambient = ambient;
  Material::diffuse = diffuse;
  Material::specular = specular;
  Material::emissive = emissive;
  Material::opacity = (_opacity.x + _opacity.y + _opacity.z) / 3.0f;
  Material::shininess = shininess;
  Material::sharpness = sharpness;
  Material::index_of_refraction = index_of_refraction;
  Material::reflection = reflection;
  //Material::alpha_texture_map = alpha_texture_map;
  Material::texture = texture;
  Material::bump_map = bump_map;
}

Material::~Material() {

}

int Material::getTexture() {
  return texture;
}
int Material::getBumpMap() {
  return bump_map;
}

glm::vec3 Material::getAmbient() const {
  return ambient;
}

glm::vec3 Material::getDiffuse() const {
  return diffuse;
}

glm::vec3 Material::getEmissive() const {
  return emissive;
}

float Material::getIndexOfRefraction() const {
  return index_of_refraction;
}

float Material::getReflection() const {
  return reflection;
}


float Material::getSharpness() const {
  return sharpness;
}

float Material::getShininess() const {
  return shininess;
}

glm::vec3 Material::getSpecular() const {
  return specular;
}

float Material::getOpacity() const {
  return opacity;
}


}
