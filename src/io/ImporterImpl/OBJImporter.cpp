// Obj_loader.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "OBJImporterImpl/obj_loader.h"

#include "OBJImporter.h"
#include "../IImporter.h"
#include "../../raytracer/utilities/Triangle.h"
#include "../../raytracer/scene/Material.h"

using namespace std;
namespace raytracer{

OBJImporter::OBJImporter()
:triangles(),materials(),point_lights(),quad_lights(),disc_lights(){

}
OBJImporter::~OBJImporter(){
	triangles.clear();
	materials.clear();

	point_lights.clear();
	quad_lights.clear();
	disc_lights.clear();
};


void OBJImporter::loadFile(char* filename){

	obj_loader *obj_data = new obj_loader();
	try{
		obj_data->load(filename);
	}
	catch(int e){
		cout << "Couldn't find file '" << filename << "'. Errmsg:" << e << endl;
	}

	for(int i=0; i<obj_data->materialCount; i++){
		obj_material *material = obj_data->materialList[i];

		std::string _name = material->name;
		glm::vec3 _ambient = vec3(material->amb[0],material->amb[1],material->amb[2]);
		glm::vec3 _diffuse = vec3(material->diff[0],material->diff[1],material->diff[2]);
		glm::vec3 _specular = vec3(material->spec[0],material->spec[1],material->spec[2]);
		glm::vec3 _emissive = vec3(material->emissive[0],material->emissive[1],material->emissive[2]);

		glm::vec3 _transparency = vec3(material->trans[0],material->trans[1],material->trans[2]);
		float _shininess = material->shiny;
		float _sharpness = material->glossy;
		float _reflection = material->reflect;
		float _index_of_reflection = material->refract_index;
		std::string _texture_map = material->texture_filename;


		OBJImporter::materials.push_back(new Material(_name,_ambient,_diffuse,_specular,_emissive,
				_transparency,_shininess,_sharpness,_reflection,_index_of_reflection,_texture_map));
	}

	// Start creating the triangles
	for(int i=0; i<obj_data->faceCount; i++){
		obj_face *face = obj_data->faceList[i];

		vector<vec3*> _vertices;
		vector<vec3*> _normals;
		vector<vec3*> _textures;
		for(int j=0;j<3;j++){
			obj_vector* _vec  = obj_data->vertexList[ face->vertex_index[j] ];
			obj_vector* _norm = obj_data->normalList[ face->normal_index[j] ];
			obj_vector* _text = obj_data->textureList[ face->texture_index[j] ];

			_vertices.push_back(new vec3(_vec->e[0],_vec->e[1],_vec->e[2]));
			_normals.push_back(new vec3(_norm->e[0],_norm->e[1],_norm->e[2]));
			//_textures.push_back(new vec3(_text->e[0],_text->e[1],_text->e[2]));
		}
		Material* _material = OBJImporter::materials[face->material_index];

		OBJImporter::triangles.push_back(new Triangle(_vertices,_normals,_textures,_material));

	}

	delete(obj_data);
}

std::vector<Triangle*>& OBJImporter::getTriangleList(){
	return OBJImporter::triangles;
}
std::vector<Material*>& OBJImporter::getMaterialList(){
	return materials;
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

std::vector<ILight*>&  OBJImporter::getLightPointList(){return OBJImporter::point_lights;}
std::vector<ILight*>&  OBJImporter::getLightQuadList(){return OBJImporter::quad_lights;}
std::vector<ILight*>&  OBJImporter::getLightDiscList(){return OBJImporter::disc_lights;}
}


