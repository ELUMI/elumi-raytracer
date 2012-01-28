/*
 * Material.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <stdint.h>
#include "../IShader.h"

namespace raytracer {

struct Color {
    uint8_t r,g,b,a;
};

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
