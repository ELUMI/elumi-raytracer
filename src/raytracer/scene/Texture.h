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

enum Corresponder { REPEAT, MIRROR, CLAMP };

class Texture {
public:
  enum TextureType { TEXTURE,HEIGHTMAP,NORMALMAP };
  enum DataType { RGB,RGBA,BGR,BGRA };
  Texture();
  Texture(unsigned int width, unsigned int height, unsigned char* data);
  Texture(unsigned int width, unsigned int height, unsigned char* data,TextureType type);
	virtual ~Texture();

	void setData(unsigned int width, unsigned int height, unsigned char* data,TextureType type);

	unsigned int getWidth();
	unsigned int getHeight();
	int getMipmapLevels();
	vec3 getColorAt(int x, int y);
	vec3 getColorAt(vec2 coords);
	vec3 getColorAt(vec2 coords, Corresponder c);
	vec3 getColorAt(vec2 coords, vec3 border_color, float scale); //Border corresponder
	vec3 getInterpolatedColor(vec2 coords);
	vec2 getUVCoordinates(vec3 point, vec3 v1v0, vec3 v2v0);
	vec2 getUVCoordinates(vec3 point, Axis axis);
	vec2 getUVCoordinates(vec3 position, vec3 normal);
	vec2 getUVCoordinatesFromSphere(vec3 normal);
	vec2 getUVCoordinatesFromSphere(vec3 normal, Axis axis);
	vec2 getUVCoordinatesFromCylinder(vec3 normal, vec3 position, Axis axis);
	void addMipmap();

private:
	vec3 repeatImage(vec2 coords);
	vec3 clampImage(vec2 coords);
	vec3 mirrorImage(vec2 coords);


  int width, height;
  unsigned char* data;
  TextureType type;
  int mipmap_levels;
};

}



#endif /* TEXTURE_H_ */
