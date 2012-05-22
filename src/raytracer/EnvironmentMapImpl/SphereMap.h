/*
 * DiffuseMap.h
 *
 *  Created on: 18 maj 2012
 *      Author: irri
 */

#ifndef DIFFUSEMAP_H_
#define DIFFUSEMAP_H_

#include "../IEnvironmentMap.h"
#include "../scene/Texture.h"

namespace raytracer {

class SphereMap: public IEnvironmentMap {
public:
  SphereMap(float*** hdrdata, unsigned int width);
  virtual ~SphereMap();

  vec3 getSpecularColor(vec3 direction);
  vec3 getDiffuseColor(vec3 normal);

private:
  float c1;// = 0.429043f;
  float c2;// = 0.511664f;
  float c3;// = 0.743125f;
  float c4;// = 0.886227f;
  float c5;// = 0.247708f;

  //Texture* texture;
  float*** data;
  unsigned int width;
  vec3 L00, L1_1, L10, L11, L2_2, L2_1, L20, L21, L22;
};

}

#endif /* DIFFUSEMAP_H_ */
