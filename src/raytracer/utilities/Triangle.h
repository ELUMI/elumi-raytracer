
#include <glm/glm.hpp>
/*
 * Triangle.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_
#include <vector>

#include "../scene/Material.h"

using namespace glm;
using namespace std;

namespace raytracer{
class Triangle {
public:
	Triangle();
	Triangle(vector<vec3*> vertices,vector<vec3*> normals,vector<vec3*> texCoords, unsigned int material);
	virtual ~Triangle();


	void set(vector<vec3*> _vertices,vector<vec3*> _normals,vector<vec3*> texCoords,
	    unsigned int material);
  void set(Triangle* copy);
	const vector<vec3*>& getVertices();
	const vector<vec3*>& getNormals();
	const vector<vec3*>& getTextures();
  unsigned int getMaterial();
  unsigned int getTexture();

protected:
	vector<vec3*> vertices;
	vector<vec3*> normals;
	vector<vec3*> texCoords;
	unsigned int material;
	unsigned int texture;
};
}
#endif /* TRIANGLE_H_ */
