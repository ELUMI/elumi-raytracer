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
    draw_data_struct = true;
    width = 400;
    height = 300;
    opengl_version = 2;
    background_color = vec4(0,0,0,0);
    use_first_bounce = false;
    wireframe = 0;
    test = 0 ;
    debug_mode=0;
    tracer = 2;
    tree = 2;
    max_recursion_depth = 5;
    recursion_attenuation_threshold = 0.015625;
  }

  int width;
  int height;
  bool draw_data_struct;
  int opengl_version;
  bool use_first_bounce;
  int wireframe;
  vec4 background_color;
  float test;
  int debug_mode;
  int tracer;
  int tree;
  unsigned int max_recursion_depth;
  float recursion_attenuation_threshold;
};

}

#endif /* SETTINGS_H_ */
