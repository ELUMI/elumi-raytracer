/*
 * Triangle.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: davesuck
 */

#include <stdlib.h>
#include <iostream>
#include "Triangle.h"

namespace raytracer{

Triangle::Triangle()
:vertices(),normals(),textures(){

}
Triangle::Triangle(vector<vec3*> vertices,vector<vec3*> normals,vector<vec3*> texture,Material* material)
:vertices(),normals(),textures(){
	Triangle::set(vertices,normals,texture,material);
}

Triangle::~Triangle() {
	Triangle::vertices.clear();
	Triangle::normals.clear();
	Triangle::textures.clear();
	delete(Triangle::material);
}

void Triangle::set(vector<vec3*> vertices,vector<vec3*> normals,vector<vec3*> textures,Material* material){
	Triangle::vertices = vertices;
	Triangle::normals = normals;
	Triangle::textures = textures;
	Triangle::material = material;
}

const vector<vec3*>& Triangle::getVertices() {return Triangle::vertices;}
const vector<vec3*>& Triangle::getNormals() {return Triangle::normals;}
const vector<vec3*>& Triangle::getTextures() {return Triangle::textures;}
const Material* Triangle::getMaterial() {return Triangle::material;}

}
