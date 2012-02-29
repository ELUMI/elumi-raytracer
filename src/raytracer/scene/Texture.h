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
  enum TextureType { TEXTURE,HEIGHTMAP,NORMALMAP };
  enum DataType { RGB,RGBA,BGR,BGRA };
  Texture();
  Texture(unsigned int width, unsigned int height, unsigned char* data);
  Texture(unsigned int width, unsigned int height, unsigned char* data,TextureType type);
	virtual ~Texture();

	unsigned int getWidth();
	unsigned int getHeight();
	vec3 getColorAt(int x, int y);
	vec3 getColorAt(vec2 coords);
	vec2 getUVCoordinates(vec3 point, vec3 e1, vec3 e2);

private:
  int width, height;
  unsigned char* data;
  TextureType type;
};

}



#endif /* TEXTURE_H_ */
