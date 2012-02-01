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
#include "../IShader.h"

namespace raytracer {

//struct Color {
//  Color() {
//    r=g=b=0;
//    a=255;
//  }
//  Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
//    r = red;
//    g = green;
//    b = blue;
//    a = alpha;
//  }
//  uint8_t r,g,b,a;
//};

class Material {

public:
  Material();
  Material(std::string name,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,
  		glm::vec3 emissive,glm::vec3 transperancy,float shininess,float sharpness,
  		float reflection ,float index_of_reflection,std::string alpha_texture_map);
  virtual ~Material();

  void setColor(glm::vec4 color);
  glm::vec4 getColor();

private:
  std::string name;

  glm::vec4 color;
  glm::vec3 ambient; 	//	Ambient
  glm::vec3 diffuse;	//	Diffuse
  glm::vec3 specular;	//	Specular
  glm::vec3 emissive;	//	Emissive

  glm::vec3 transperancy;	//	Transperancy (Tr)
  float shininess;			//	Shininess
  float index_of_reflection;// --||--
  float reflection;
  float sharpness;			// 	Shartness
  int illumination_model; 	//  Illumination model used

  std::string ambient_texture_map;	// Filename of ambient texture map
  std::string diffuse_texture_map;	// Filename of diffuse texture map
  std::string specular_texture_map; // Filename of specular texture map
  std::string alpha_texture_map;	// Filename of alpha texture map
  std::string bump_texture_map;		// Filename of bump texture map

  IShader* shader;

};

}

#endif /* MATERIAL_H_ */
