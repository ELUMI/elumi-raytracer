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
    // Screen
    width = 400;
    height = 300;

    opengl_version = 2;
    background_color = vec4(0,0,0,0);
    use_first_bounce = false;
    test=0;
    debug_mode=0;

    // Tracer
    tracer = 2;

    // Recursion
    max_recursion_depth = 5;
    recursion_attenuation_threshold = 0.015625;

    // Threading
    threads = 0;  // auto

    // Tonemapping
    key = 0.18;
    white = 1e20;
  }

  // Screen
  int width;
  int height;

  int opengl_version;
  bool use_first_bounce;
  vec4 background_color;
  float test;
  int debug_mode;

  // Tracer
  int tracer;

  // Recursion
  unsigned int max_recursion_depth;
  float recursion_attenuation_threshold;

  // Threading
  int threads;

  // Tonemapping
  float key;
  float white;
};

}

#endif /* SETTINGS_H_ */
