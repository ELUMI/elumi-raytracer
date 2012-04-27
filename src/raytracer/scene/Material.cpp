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
  Material::name                = "default_material";
  Material::ambient             = glm::vec3(0.2f,0.2f,0.2f);
  Material::diffuse             = glm::vec3(0.8f,0.8f,0.8f);
  Material::specular            = glm::vec3(0.2f,0.2f,0.2f);
  Material::emissive            = glm::vec3(0.0f,0.0f,0.0f); //!?!
  Material::opacity             = 1.0f;
  Material::shininess           = 0.0f;
  Material::sharpness           = 98; //glossy
  Material::reflection          = 0.0f;
  Material::index_of_refraction = 1.0f;
  Material::diffuse_map         = -1;
  Material::bump_map            = -1;
  Material::specular_map        = -1;
  Material::transparency_map    = -1;
  Material::reflection_map      = -1;
  Material::reflect_spread      = 0;
  Material::reflect_samples     = 0;
  Material::reflect_spread      = 0;
  Material::refract_samples     = 0;
  Material::projector           = CUBE;
  Material::corresponder        = REPEAT;
  Material::scale               = 1.0f;
  Material::use_relief          = false;
}

Material::Material(std::string name,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,
    glm::vec3 emissive,glm::vec3 _opacity,float shininess,float sharpness,
    float reflection ,float index_of_refraction, int diffuse_map, int bump_map,
    int norm_map, int specular_map, int r_map, int d_map,
    float reflect_spread, int reflect_samples,
    float refract_spread, int refract_samples, Projector projector,
    Corresponder corresponder, float scale, bool use_relief){

  Material::name                = name;
  Material::ambient             = ambient;
  Material::diffuse             = diffuse;
  Material::specular            = specular;
  Material::emissive            = emissive;
  Material::opacity             = (_opacity.x + _opacity.y + _opacity.z) / 3.0f;
  Material::shininess           = shininess;
  Material::sharpness           = sharpness;
  Material::index_of_refraction = index_of_refraction;
  Material::reflection          = reflection;
  //Material::alpha_texture_map = alpha_texture_map;
  Material::diffuse_map = diffuse_map;
  Material::bump_map = bump_map;
  Material::normal_map = norm_map;
  Material::specular_map = specular_map;
  Material::transparency_map = d_map;
  Material::reflection_map = r_map;
  Material::reflect_spread      = reflect_spread;
  Material::reflect_samples     = reflect_samples;
  Material::refract_spread      = refract_spread;
  Material::refract_samples     = refract_samples;
  Material::projector           = projector;
  Material::corresponder        = corresponder;
  Material::scale               = scale;
  Material::use_relief          = use_relief;
}

Material::~Material() {

}

int Material::getDiffuseMap() {
  return diffuse_map;
}

int Material::getSpecularMap() {
  return specular_map;
}

int Material::getBumpMap() {
  return bump_map;
}

int Material::getTransparencyMap() {
  return transparency_map;
}

int Material::getReflectionMap() {
  return reflection_map;
}

int Material::getNormalMap() {
  return normal_map;
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

float Material::getReflectionSpread() const {
  return reflect_spread;
}

int Material::getReflectionSamples() const {
  return reflect_samples;
}

float Material::getRefractionSpread() const {
  return refract_spread;
}

int Material::getRefractionSamples() const {
  return refract_samples;
}

Projector Material::getProjector() const {
  return projector;
}

Corresponder Material::getCorresponder() const {
  return corresponder;
}

float Material::getScale() const {
  return scale;
}

bool Material::getUseRelief() const {
  return use_relief;
}


}
