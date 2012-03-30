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

enum Axis { XAXIS, YAXIS, ZAXIS };

class Texture {
public:
  enum TextureType { TEXTURE,HEIGHTMAP,NORMALMAP };
  enum DataType { RGB,RGBA,BGR,BGRA };
  Texture(unsigned int width, unsigned int height, unsigned char* data);
  Texture(unsigned int width, unsigned int height, unsigned char* data,TextureType type);
	virtual ~Texture();

	unsigned int getWidth();
	unsigned int getHeight();
	int getMipmapLevels();
	vec3 getColorAt(int x, int y);
	vec3 getColorAt(vec2 coords);
	vec3 getInterpolatedColor(vec2 coords);
	vec2 getUVCoordinates(vec3 point, vec3 v1v0, vec3 v2v0);
	vec2 getUVCoordinates(vec3 point, Axis axis);
	void addMipmap();

private:
  int width, height;
  unsigned char* data;
  TextureType type;
  int mipmap_levels;
};

}



#endif /* TEXTURE_H_ */
