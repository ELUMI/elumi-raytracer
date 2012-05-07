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

enum Projector { CUBE, PLANE, SPHERE, CYLINDER, UV };

enum Corresponder { REPEAT, MIRROR, CLAMP, BORDER };

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
	vec3 getColorAt(vec2 coords, float scale, Corresponder c);
	vec3 getColorAt(vec2 coords, vec3 border_color, float scale); //Border corresponder
	vec3 getInterpolatedColor(vec2 coords);
	vec2 getUVCoordinates(vec3 point, vec3 v1v0, vec3 v2v0);
	static vec2 getUVCoordinates(vec3 position, vec3 normal, double scale, Projector projector, Axis axis, bool use_position);

	void addMipmap();

	//Projector functions
	static vec2 cubeMapping(vec3 position, vec3 normal);
	static vec2 planeMapping(vec3 point, Axis axis);
	static vec2 sphereMapping(vec3 normal, Axis axis);
	static vec2 cylinderMapping(vec3 normal, vec3 position, Axis axis);

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
