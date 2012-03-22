/*
 * Settings.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <glm/glm.hpp>
#include <stdint.h>

using namespace glm;

namespace raytracer {

struct Settings {

  Settings() {
    width = 640;
    height = 480;
    opengl_version = 3;
    background_color = vec4(0,0,0,0);
    use_first_bounce = false;
    test=0;
    debug_mode=0;
    tracer = 2;
    max_recursion_depth = 5;
    recursion_attenuation_threshold = 0.015625;
  }

  int width;
  int height;
  int opengl_version;
  bool use_first_bounce;
  vec4 background_color;
  float test;
  int debug_mode;
  int tracer;
  unsigned int max_recursion_depth;
  float recursion_attenuation_threshold;
};

}

#endif /* SETTINGS_H_ */
