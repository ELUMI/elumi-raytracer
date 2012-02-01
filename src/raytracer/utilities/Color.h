/*
 * Color.h
 *
 *  Created on: Feb 1, 2012
 *      Author: irri
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

namespace raytracer {

class Color {
public:
  Color();
  Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
  virtual ~Color();
  uint8_t r,g,b,a;
};

}

#endif /* COLOR_H_ */
