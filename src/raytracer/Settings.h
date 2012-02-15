/*
 * Settings.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define DEFAULT_PIXEL_BITS 4 // RGBA

#include <glm/glm.hpp>
#include <stdint.h>

using namespace glm;

namespace raytracer {

struct Settings {

  Settings() {
    width = 640;
    height = 480;

    use_opengl = true;

    backgroundColor = vec4(0,0,0,1);
  }

  int width;
  int height;
  vec4 backgroundColor;
  bool use_opengl;
};

}

#endif /* SETTINGS_H_ */
