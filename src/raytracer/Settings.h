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
    width = 320;
    height = 240;
    use_opengl = true;
    draw_data_struct = true;
    background_color = vec4(0,0,0,1);
    use_first_bounce = false;
    test = 0 ;
    debug_mode=0;
  }

  int width;
  int height;
  bool use_opengl;
  bool draw_data_struct;
  bool use_first_bounce;
  vec4 background_color;
  float test;
  int debug_mode;
};

}

#endif /* SETTINGS_H_ */
