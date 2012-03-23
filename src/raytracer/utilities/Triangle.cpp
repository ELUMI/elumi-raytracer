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

	Triangle::barycenter = vec3((vertices[0]->x + vertices[1]->x + vertices[2]->x)/3
	    ,(vertices[0]->y + vertices[1]->y + vertices[2]->y)/3
	    ,(vertices[0]->z + vertices[1]->z + vertices[2]->z)/3);

	min = vec3((*vertices[0])[0],(*vertices[0])[1],(*vertices[0])[2]);

	max = vec3((*vertices[0])[0],(*vertices[0])[1],(*vertices[0])[2]);
	for(size_t j=1;j<3;j++){
	  for(size_t i=0;i<3;i++){
	    float _value = (*vertices[j])[i];
	    if(_value<min[i])
	      min[i] = _value;
      if(_value>max[i])
        max[i] = _value;
	  }
	}
}
void Triangle::set(Triangle* copy){
  vertices.assign(copy->vertices.begin(),copy->vertices.end());
  normals.assign(copy->normals.begin(),copy->normals.end());
  texCoords.assign(copy->texCoords.begin(),copy->texCoords.end());
  material = copy->material;
  min = copy->min;
  max = copy->max;
//  max[0]=copy->max[0]; max[1]=copy->max[1]; max[2]=copy->max[2];
//  min[0]=copy->min[0]; min[1]=copy->min[1]; min[2]=copy->min[2];
}

const vector<vec3*>& Triangle::getVertices() {return Triangle::vertices;}
const vector<vec3*>& Triangle::getNormals() {return Triangle::normals;}
const vector<vec3*>& Triangle::getTextures() {return Triangle::texCoords;}
const float& Triangle::getMin(int axis){return min[axis];}
const float& Triangle::getMax(int axis){return max[axis];}
unsigned int Triangle::getMaterial() {return Triangle::material;}
unsigned int Triangle::getTexture() {return Triangle::texture;}
const float& Triangle::getBarycenter(int axis){return Triangle::barycenter[axis];}

}
