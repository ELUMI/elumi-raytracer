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
	Triangle::vertices.clear();
	Triangle::normals.clear();
	Triangle::texCoords.clear();
	//delete(Triangle::material); //TODO: Add delete in scene
}

void Triangle::set(vector<vec3*> vertices, vector<vec3*> normals, vector<vec3*> texCoords,
    unsigned int material){
	Triangle::vertices = vertices;
	Triangle::normals = normals;
	Triangle::texCoords = texCoords;
	Triangle::material = material;
	Triangle::texture = texture;

	min = new float[3];
	max = new float[3];
	min[0] = (*vertices[0])[0];min[1] = (*vertices[0])[1]; min[2] = (*vertices[0])[2];

	max[0] = (*vertices[0])[0]; max[1] = (*vertices[0])[1]; max[3] = (*vertices[0])[2];
	for(size_t j=1;j<3;j++){
	  for(size_t i=0;i<3;i++){
	    float _min = (*vertices[j])[i];
	    if(min[i]<_min)
	      min[i] = _min;
	  }
	  for(size_t i=0;i<3;i++){
	    float _max = (*vertices[j])[i];
	    if(max[i]<_max)
	      max[i] = _max;
	  }
	}
}

const vector<vec3*>& Triangle::getVertices() {return Triangle::vertices;}
const vector<vec3*>& Triangle::getNormals() {return Triangle::normals;}
const vector<vec3*>& Triangle::getTextures() {return Triangle::texCoords;}
const float& Triangle::getMin(int axis){return min[axis];}
const float& Triangle::getMax(int axis){return max[axis];}
unsigned int Triangle::getMaterial() {return Triangle::material;}
unsigned int Triangle::getTexture() {return Triangle::texture;}

}
