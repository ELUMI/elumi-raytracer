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
      glm::vec3 emissive,glm::vec3 transperancy,float shininess,float sharpness,
      float reflection ,float index_of_refraction, int texture,int bump_map);
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
  float getTransparency() const;
  int getTexture();
  int getBumpMap();


private:
  std::string name;

  glm::vec3 ambient; 	//	Ambient
  glm::vec3 diffuse;	//	Diffuse
  glm::vec3 specular;	//	Specular
  glm::vec3 emissive;	//	Emissive

  float transparency;	//	Transparency (Tr)
  float shininess;			//	Shininess
  float index_of_refraction;// --||--
  float reflection;
  float sharpness;			// 	Sharpness

  int texture;
  int bump_map;
};

}

#endif /* MATERIAL_H_ */
