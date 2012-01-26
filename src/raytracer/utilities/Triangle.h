
#include "glm.hpp"
/*
 * Triangle.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "..\scene\Texture.h"
#include "..\scene\Material.h"

using namespace glm;

namespace raytracer{
class Triangle {
public:
	Triangle();
	Triangle(vec3* vertices,vec3* normals,Texture* texture,Material* material);
	virtual ~Triangle();

	void set(vec3* _vertices,vec3* _normals,Texture* _texture,Material* material);
	vec3* getVertices();
	vec3* getNormals();
	Texture* getTexture();
	Material* getMaterial();

private:
	vec3* vertices;
	vec3* normals;
	Texture* texture;
	Material* material;
};
}
#endif /* TRIANGLE_H_ */
