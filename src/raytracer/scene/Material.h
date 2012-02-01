/*
 * Material.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <stdint.h>
//#include <glm/glm.hpp>
#include "../utilities/Color.h"
#include "../IShader.h"

namespace raytracer {

//struct Color {
//  Color() {
//    r=g=b=0;
//    a=255;
//  }
//  Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
//    r = red;
//    g = green;
//    b = blue;
//    a = alpha;
//  }
//  uint8_t r,g,b,a;
//};

class Material {

public:
  Material();
  virtual ~Material();

  Color getColor();

private:
  Color color;

  IShader* shader;

};

}

#endif /* MATERIAL_H_ */
