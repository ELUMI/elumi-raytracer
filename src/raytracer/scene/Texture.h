/*
 * Texture.h
 *
 *  Created on: Jan 26, 2012
 *      Author: DmZ
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <glm/glm.hpp>

using namespace glm;

namespace raytracer {

class Texture {
public:
  Texture();
  Texture(unsigned int width, unsigned int height, unsigned char* data);
	virtual ~Texture();

	unsigned int getWidth();
	unsigned int getHeight();
	vec3 getColorAt(int x, int y);

private:
  int width, height;
  unsigned char* data;
};

}



#endif /* TEXTURE_H_ */
