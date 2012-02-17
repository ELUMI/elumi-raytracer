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
  Material::transparency = 1.0f;
  Material::shininess = 0;
  Material::sharpness = 98; //glossy
  Material::index_of_reflection = 1;
  Material::reflection = 0.0f;
  Material::alpha_texture_map = "\0";
  Material::texture = -1;
}

Material::Material(std::string name,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,
    glm::vec3 emissive,glm::vec3 transperency,float shininess,float sharpness,
    float reflection ,float index_of_reflection,std::string alpha_texture_map, int texture){

  Material::name = name;
  Material::ambient = ambient;
  Material::diffuse = diffuse;
  Material::specular = specular;
  Material::emissive = emissive;
  Material::transparency = (transperency.x + transperency.y + transperency.z) / 3.0f;
  Material::shininess = shininess;
  Material::sharpness = sharpness;
  Material::index_of_reflection = index_of_reflection;
  Material::reflection = reflection;
  Material::alpha_texture_map = alpha_texture_map;
  Material::texture=texture;
}

Material::~Material() {

}

int Material::getTexture() {
  return texture;
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

int Material::getIllumination_model() const {
  return illumination_model;
}

float Material::getIndex_of_reflection() const {
  return index_of_reflection;
}

float Material::getReflection() const {
  return reflection;
}

IShader *Material::getShader() const {
  return shader;
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

float Material::getTransparency() const {
  return transparency;
}


}
