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
      float reflection ,float index_of_refraction, int diffuse_map,int bump_map,
      int norm_map, int specular_map,int r_map, int d_map,
      float reflect_spread, int reflect_samples,
      float refract_spread, int refract_samples, Projector projector,
      Corresponder corresponder,float scale, bool use_relief);
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
  int getTransparencyMap();
  int getSpecularMap();
  int getReflectionMap();
  int getBumpMap();
  int getNormalMap();
  float getOpacity() const;

  float getReflectionSpread() const;
  int getReflectionSamples() const;
  float getRefractionSpread() const;
  int getRefractionSamples() const;

  Projector getProjector() const;
  Corresponder getCorresponder() const;
  float getScale() const;
  bool getUseRelief() const;

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

  float reflect_spread;     // (r_spread)
  int reflect_samples;    // (r_samples)
  float refract_spread;     // (Ni_spread)
  int refract_samples;    // (Ni_samples)

  int diffuse_map;
  int bump_map;
  int normal_map;
  int specular_map;
  int transparency_map;
  int reflection_map;

  float scale;
  Projector projector;
  Corresponder corresponder;

  bool use_relief;
};

}

#endif /* MATERIAL_H_ */
