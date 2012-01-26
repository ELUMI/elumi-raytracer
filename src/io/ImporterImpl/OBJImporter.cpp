// Obj_loader.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "OBJImporterImpl/obj_loader.h"

#include "OBJImporter.h"
#include "../IImporter.h"
#include "../../raytracer/utilities/Triangle.h"

using namespace std;
namespace raytracer{

IImporter::~IImporter() {

};
OBJImporter::~OBJImporter(){
	free(triangles);
};


void OBJImporter::loadFile(char* filename){

	obj_loader *obj_data = new obj_loader();
	try{
		obj_data->load(filename);
	}
	catch(int e){
		cout << "Couldn't find file '" << filename << "'. Errmsg:" << e << endl;
	}

	triangles = new Triangle*[obj_data->faceCount];
	// Start creating the triangles
	for(int i=0; i<obj_data->faceCount; i++){
		obj_face *face = obj_data->faceList[i];
		vec3* _vertices = new vec3[3];
		vec3* _normals = new vec3[3];
		vec3* _textures = new vec3[3];
		for(int j=0;j<3;j++){
			obj_vector* _vec  = obj_data->vertexList[ face->vertex_index[j] ];
			obj_vector* _norm = obj_data->normalList[ face->normal_index[j] ];
			obj_vector* _text = obj_data->textureList[ face->texture_index[j] ];
			_vertices[j] = vec3(_vec->e[0],_vec->e[1],_vec->e[2]);
			_normals[j] = vec3(_norm->e[0],_norm->e[1],_norm->e[2]);
			_textures[j] = vec3(_text->e[0],_text->e[1],_text->e[2]);
		}
		//obj_material* _material = obj_data->materialList[face->material_index];

		triangles[i] = new Triangle(_vertices,_normals,_textures,NULL);;

	}
	triangle_count = obj_data->faceCount;

	materials = new Material*[obj_data->materialCount];
	for(int i=0; i<obj_data->materialCount; i++){
		obj_material *material;
		Material* _material;

		materials[i] = _material;
	}
	material_count = obj_data->materialCount;
}

Triangle** OBJImporter::gerTriangleList(){
	if (triangles!=NULL){
		return triangles;
	}
	else{
		cout << "Triangle NULL";
		// Throw exception
		return NULL;
	}
}
int OBJImporter::getTriangleCount(){
	if(triangles!=NULL){
		return triangle_count;
	}
	else{
		return -1;
	}
}
Material** OBJImporter::getMaterialList(){
	if (materials!=NULL){
		return materials;
	}
	else{
		cout << "Materials NULL";
		// Throw exception
		return NULL;
	}
}
int OBJImporter::getMaterialCount(){
	if(materials!=NULL){
		return material_count;
	}
	else{
		return -1;
	}
}
Camera* OBJImporter::getCamera(){
	if(camera!=NULL){
		return camera;
	}
	else{
		//Throw exception
		return NULL;
	}
}

int OBJImporter::getLightPointList(){return -1;}
int OBJImporter::getLightQuadList(){return -1;}
int OBJImporter::getLightDiscList(){return -1;}
}


