/*
 * Settings.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define DEFAULT_PIXEL_BITS 4 // RGBA

#include <stdint.h>
#include <iostream>

namespace raytracer {

struct Settings {

  Settings() {
    width = 640;
    height = 480;

    backgroundColor = new uint8_t[DEFAULT_PIXEL_BITS];

    backgroundColor[0] = 0;
    backgroundColor[1] = 0;
    backgroundColor[2] = 0;
    backgroundColor[3] = 255;

  }

  ~Settings() {
    delete [] backgroundColor;
  }

  int width;
  int height;
  uint8_t* backgroundColor;

};

}

#endif /* SETTINGS_H_ */
