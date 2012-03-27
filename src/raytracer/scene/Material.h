/*
 * Material.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <stdint.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Texture.h"

namespace raytracer {

class Material {

public:
  Material();
  Material(std::string name,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,
      glm::vec3 emissive,glm::vec3 opacity,float shininess,float sharpness,
      float reflection ,float index_of_refraction, int diffuse_map,int bump_map,int specular_map);
  virtual ~Material();

  glm::vec3 getColor();
  glm::vec3 getAmbient() const;
  glm::vec3 getDiffuse() const;
  glm::vec3 getEmissive() const;
  float getIndexOfRefraction() const;
  float getReflection() const;
  float getSharpness() const;
  float getShininess() const;
  glm::vec3 getSpecular() const;
  int getDiffuseMap();
  int getSpecularMap();
  int getBumpMap();
  float getOpacity() const;


private:
  std::string name;

  glm::vec3 ambient; 	//	Ambient
  glm::vec3 diffuse;	//	Diffuse
  glm::vec3 specular;	//	Specular
  glm::vec3 emissive;	//	Emissive


  float opacity;	          // (d)
  float shininess;			    // (Ns)
  float index_of_refraction;// (Ni)
  float reflection;         // (r)
  float sharpness;			    // ???????????????????????????????????????????????

  int diffuse_map;
  int bump_map;
  int specular_map;

};

}

#endif /* MATERIAL_H_ */
