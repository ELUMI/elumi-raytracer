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
  for(size_t i=0; i<vertices.size(); ++i)
    delete vertices.at(i);
  for(size_t i=0; i<normals.size(); ++i)
      delete normals.at(i);
  for(size_t i=0; i<texCoords.size(); ++i)
      delete texCoords.at(i);
}

void Triangle::set(vector<vec3*> vertices, vector<vec3*> normals, vector<vec3*> texCoords,
    unsigned int material){
	Triangle::vertices = vertices;
	Triangle::normals = normals;
	Triangle::texCoords = texCoords;
	Triangle::material = material;
	Triangle::texture = texture;
}
void Triangle::set(Triangle* copy){
  vertices.assign(copy->vertices.begin(),copy->vertices.end());
  normals.assign(copy->normals.begin(),copy->normals.end());
  texCoords.assign(copy->texCoords.begin(),copy->texCoords.end());
  material = copy->material;
}

const vector<vec3*>& Triangle::getVertices() {return vertices;}
const vector<vec3*>& Triangle::getNormals() {return normals;}
const vector<vec3*>& Triangle::getTextures() {return texCoords;}
unsigned int Triangle::getMaterial() {return Triangle::material;}
unsigned int Triangle::getTexture() {return Triangle::texture;}

}
