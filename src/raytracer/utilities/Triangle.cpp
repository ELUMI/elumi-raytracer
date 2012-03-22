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
:vertices(),normals(),texCoords(){

}
Triangle::Triangle(vector<vec3*> vertices, vector<vec3*> normals, vector<vec3*> texCoords,
    unsigned int material)
:vertices(),normals(),texCoords(){
	Triangle::set(vertices,normals,texCoords,material);
}

Triangle::~Triangle() {
  //TODO ta bort alla verts etc.
	//delete(Triangle::material); //TODO: Add delete in scene
}

void Triangle::set(vector<vec3*> vertices, vector<vec3*> normals, vector<vec3*> texCoords,
    unsigned int material){
	Triangle::vertices = vertices;
	Triangle::normals = normals;
	Triangle::texCoords = texCoords;
	Triangle::material = material;
	Triangle::texture = texture;
}

const vector<vec3*>& Triangle::getVertices() {return Triangle::vertices;}
const vector<vec3*>& Triangle::getNormals() {return Triangle::normals;}
const vector<vec3*>& Triangle::getTextures() {return Triangle::texCoords;}
unsigned int Triangle::getMaterial() {return Triangle::material;}
unsigned int Triangle::getTexture() {return Triangle::texture;}

}
