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
	free(vertices);
	free(normals);
	free(texture);
	free(material);
}

void Triangle::set(vec3* vertices,vec3* normals,vec3* texture,Material* material){
	this->vertices = vertices;
	this->normals = normals;
	this->texture = texture;
	this->material = material;
}
vec3* Triangle::getVertices() {return vertices;}
vec3* Triangle::getNormals() {return normals;}
vec3* Triangle::getTexture() {return texture;}
Material* Triangle::getMaterial() {return material;}

}
