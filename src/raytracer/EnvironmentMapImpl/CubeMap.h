/*
 * CubeMap.h
 *
 *  Created on: Apr 6, 2012
 *      Author: irri
 */

#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include "../IEnvironmentMap.h"
#include "../scene/Texture.h"

namespace raytracer {

class CubeMap : public IEnvironmentMap {
public:
  CubeMap();
  CubeMap(Texture** textures, size_t size);
  virtual ~CubeMap();

  vec3 getSpecularColor(vec3 direction);
  vec3 getDiffuseColor(vec3 normal);

private:
  void loadTextures(Texture** textures, size_t size);
  vec3 getColorFromImage(vec2 coord, int quad);

  Texture** textures;
  static const size_t nr_of_images = 6;
};

} /* namespace raytracer */
#endif /* CUBEMAP_H_ */
