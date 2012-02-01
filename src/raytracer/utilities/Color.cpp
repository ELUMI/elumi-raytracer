/*
 * Color.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: irri
 */

#include "Color.h"

namespace raytracer {

Color::Color() {
  r=g=b=0;
  a=255;
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
  r = red;
  g = green;
  b = blue;
  a = alpha;
}

Color::~Color() {
  // TODO Auto-generated destructor stub
}

}
