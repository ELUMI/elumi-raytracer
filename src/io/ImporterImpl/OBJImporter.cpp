// Obj_loader.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "OBJImporter.h"
#include "../IImporter.h"
#include "../../raytracer/utilities/Triangle.h"
#include "OBJImporterUtil/objLoader.h"

using namespace std;
namespace raytracer{

IImporter::~IImporter() {

};
OBJImporter::~OBJImporter(){
	free(triangles);
};


void OBJImporter::loadFile(char* filename){

	objLoader *obj_data = new objLoader();
	try{
		obj_data->load(filename);
	}
	catch(int e){
		cout << "Couldn't find file '" << filename << "'. Errmsg:" << e << endl;
	}

	triangles = new Triangle[obj_data->faceCount];
	// Start creating the triangles
	for(int i=0; i<obj_data->faceCount; i++){
		obj_face *face = obj_data->faceList[i];
		Triangle _triangle;


		//objData->vertexList[ face->vertex_index[j] ]

		triangles[i] = _triangle;
	}
	triangle_count = obj_data->faceCount;

	materials = new Material[obj_data->materialCount];
	for(int i=0; i<obj_data->materialCount; i++){
		obj_material *material;
		Material _material;

		materials[i] = _material;
	}
	material_count = obj_data->materialCount;
}

Triangle* OBJImporter::gerTriangleList(){
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
Material* OBJImporter::getMaterialList(){
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


