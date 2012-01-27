/*
 * Triangle.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: davesuck
 */

#include <stdlib.h>
#include "Triangle.h"
namespace raytracer{

Triangle::Triangle() {
	// TODO Auto-generated constructor stub

}
Triangle::Triangle(vec3* vertices,vec3* normals,vec3* texture,Material* material){
	Triangle::set(vertices,normals,texture,material);
}

Triangle::~Triangle() {
	delete(Triangle::vertices);
	delete(Triangle::normals);
	delete(Triangle::texture);
	delete(Triangle::material);
}

void Triangle::set(vec3* vertices,vec3* normals,vec3* texture,Material* material){
	Triangle::vertices = vertices;
	Triangle::normals = normals;
	Triangle::texture = texture;
	Triangle::material = material;
}
const vec3* Triangle::getVertices() {return Triangle::vertices;}
const vec3* Triangle::getNormals() {return Triangle::normals;}
const vec3* Triangle::getTexture() {return Triangle::texture;}
const Material* Triangle::getMaterial() {return Triangle::material;}

}
