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

  void loadTextures(Texture** textures, size_t size);
  vec3 getColor(Ray ray);

private:
  vec3 getColorFromImage(vec2 coord, int quad);

  //vec4*** image_data;
  Texture** textures;
  static const size_t nr_of_images = 6;
  //int width, height;
};

} /* namespace raytracer */
#endif /* CUBEMAP_H_ */
