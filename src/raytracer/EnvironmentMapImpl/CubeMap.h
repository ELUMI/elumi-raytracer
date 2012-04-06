/*
 * CubeMap.h
 *
 *  Created on: Apr 6, 2012
 *      Author: irri
 */

#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include "../IEnvironmentMap.h"

namespace raytracer {

class CubeMap : public IEnvironmentMap {
public:
  CubeMap();
  virtual ~CubeMap();

  void loadImageFiles(const char** filenames, int length);
  vec4 getColor(Ray ray);

private:
  vec4 getColorFromImage(vec2 coord, int quad);

  vec4*** image_data;
  static const size_t nr_of_images = 6;
  int width, height;
};

} /* namespace raytracer */
#endif /* CUBEMAP_H_ */
