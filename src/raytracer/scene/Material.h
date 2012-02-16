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

namespace raytracer {

class IShader;

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
      float reflection ,float index_of_reflection,std::string alpha_texture_map,int texture);
  virtual ~Material();

  glm::vec3 getColor();
  glm::vec3 getAmbient() const;
  glm::vec3 getDiffuse() const;
  glm::vec3 getEmissive() const;
  int getIllumination_model() const;
  float getIndex_of_reflection() const;
  float getReflection() const;
  float getSharpness() const;
  float getShininess() const;
  glm::vec3 getSpecular() const;
  float getTransparency() const;

  IShader *getShader() const;

private:
  std::string name;

  glm::vec3 ambient; 	//	Ambient
  glm::vec3 diffuse;	//	Diffuse
  glm::vec3 specular;	//	Specular
  glm::vec3 emissive;	//	Emissive

  float transparency;	//	Transparency (Tr)
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

  int texture; //Texture index


};

}

#endif /* MATERIAL_H_ */
