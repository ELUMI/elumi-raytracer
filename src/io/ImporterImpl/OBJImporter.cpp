// Obj_loader.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include "OBJImporterImpl/obj_loader.h"
#include "../../raytracer/common.hpp"


#include <IL/il.h>
#include <IL/ilu.h>

#include "OBJImporter.h"
#include "../IImporter.h"
#include "../../raytracer/utilities/Triangle.h"
#include "../../raytracer/scene/Material.h"

using namespace std;
namespace raytracer{

OBJImporter::OBJImporter()
:triangles(),materials(),textures(),point_lights(),quad_lights(),disc_lights(){

}
OBJImporter::~OBJImporter(){

  //TODO

  /* should this be done? there is a leak here, but it is small...
  for(size_t i=0; i<triangles.size(); ++i)
    delete triangles[i];

  for(size_t i=0; i<materials.size(); ++i)
    delete materials[i];

  for(size_t i=0; i<textures.size(); ++i)
    delete textures[i];
  */

	triangles.clear();
	materials.clear();
	textures.clear();

	point_lights.clear();
	quad_lights.clear();
	disc_lights.clear();
};

void OBJImporter::loadFile(const char* filename){

  ilInit();
  //iluInit();

	obj_loader *obj_data = new obj_loader();

	try{
		obj_data->load(filename);
	}
	catch(int e){
		cout << "Couldn't find file '" << filename << "'. Errmsg:" << e << endl;
	}

	int materialOffset = materials.size();

	for(int i=0; i<obj_data->materialCount; i++){
		obj_material *material = obj_data->materialList[i];

		std::string _name           = material->name;
		glm::vec3 _ambient          = vec3(material->amb[0],material->amb[1],material->amb[2]);
		glm::vec3 _diffuse          = vec3(material->diff[0],material->diff[1],material->diff[2]);
		glm::vec3 _specular         = vec3(material->spec[0],material->spec[1],material->spec[2]);
		glm::vec3 _emissive         = vec3(material->emissive[0],material->emissive[1],material->emissive[2]);

		glm::vec3 _transparency     = vec3(material->trans[0],material->trans[1],material->trans[2]);
		float _shininess            = material->shiny;
		float _sharpness            = material->glossy;
		float _reflection           = material->reflect;
		float _index_of_refraction  = material->refract_index;
		float fresnel         = material->fresnel;
		float _refraction           = material->refract;
		float _reflect_spread       = material->reflect_spread;
		int   _reflect_samples      = material->reflect_samples;
		float _refract_spread       = material->refract_spread;
		int   _refract_samples      = material->refract_samples;
		float _scale                = material->scale;
		int projector               = material->projector;
		int axis               = material->axis;
		int corresponder            = material->corresponder;
		bool use_relief             = material->relief;
		bool use_position           = material->use_position;
		std::string _diffuse_map    = material->diffuse_map;
		std::string _bump_map       = material->bump_filename;
		std::string _norm_map       = material->norm_filename;
		std::string _ks_map         = material->ks_filename;
		std::string _d_map          = material->d_filename;
		std::string _r_map          = material->r_filename;

		Projector _projector         = CUBE;
		Corresponder _corresponder   = REPEAT;
		Axis _axis                    = YAXIS;

    replace(_diffuse_map.begin(), _diffuse_map.end(), '\n', '\0');
    replace(_bump_map.begin(), _bump_map.end(), '\n', '\0');
    replace(_norm_map.begin(), _norm_map.end(), '\n', '\0');
    replace(_ks_map.begin(), _ks_map.end(), '\n', '\0');
    replace(_d_map.begin(), _d_map.end(), '\n', '\0');
    replace(_r_map.begin(), _r_map.end(), '\n', '\0');

		ILuint image;
		int diff_map_index = -1, bump_map = -1, ks_map = -1, norm_map = -1,
		    d_map = -1, r_map = -1;

		//Axis
		switch (axis) {
      case 0:
        _axis = XAXIS;
        break;
      case 1:
        _axis = YAXIS;
        break;
      case 2:
        _axis = ZAXIS;
        break;
      default:
        _axis = YAXIS;
        break;
    }

		//Projector function
		switch (projector) {
      case 0:
        _projector = PLANE;
        break;
      case 1:
        _projector = CUBE;
        break;
      case 2:
        _projector = SPHERE;
        break;
      case 3:
        _projector = CYLINDER;
        break;
      default:
        _projector = UV;
        break;
    }

		//Corresponder function
		switch (corresponder) {
      case 0:
        _corresponder = REPEAT;
        break;
      case 1:
        _corresponder = MIRROR;
        break;
      case 2:
        _corresponder = CLAMP;
        break;
      case 3:
        _corresponder = BORDER;
        break;
      default:
        _corresponder = REPEAT;
        break;
    }

		//Texture
    if(!_diffuse_map.empty()) {

      image = ilGenImage();
      ilBindImage(image);

      ilLoadImage(_diffuse_map.c_str());

      if(ilGetError() == IL_NO_ERROR) {
        cout << "Image loaded" << endl;
        ILuint w,h;

        w = ilGetInteger(IL_IMAGE_WIDTH);
        h = ilGetInteger(IL_IMAGE_HEIGHT);

        textures.push_back(new Texture(w,h,ilGetData()));
        diff_map_index = textures.size()-1;

      } else {
        //Image not loaded (?)
        cout << "Image not loaded" << endl;
      }
    }

    //Bump map
    if(!_bump_map.empty()) {
      image = ilGenImage();
      ilBindImage(image);

      ilLoadImage(_bump_map.c_str());

      if(ilGetError() == IL_NO_ERROR) {
        ILuint w,h;

        w = ilGetInteger(IL_IMAGE_WIDTH);
        h = ilGetInteger(IL_IMAGE_HEIGHT);

        textures.push_back(new Texture(w,h,ilGetData()));
        bump_map = textures.size()-1;
      } else {
        //Image not loaded (?)
        cout << "Bumpmap not loaded" << endl;
      }
    }

    //Normal map
    if(!_norm_map.empty()) {
      image = ilGenImage();
      ilBindImage(image);

      ilLoadImage(_norm_map.c_str());

      if(ilGetError() == IL_NO_ERROR) {
        cout << "Normal map loaded " << _norm_map << endl;
        ILuint w,h;

        w = ilGetInteger(IL_IMAGE_WIDTH);
        h = ilGetInteger(IL_IMAGE_HEIGHT);

        textures.push_back(new Texture(w,h,ilGetData()));
        norm_map = textures.size()-1;
      } else {
        //Image not loaded (?)
        cout << "Normal map not loaded" << endl;
      }
    }

    //Specular map
    if(!_ks_map.empty()) {
      image = ilGenImage();
      ilBindImage(image);

      ilLoadImage(_ks_map.c_str());

      if(ilGetError() == IL_NO_ERROR) {
        ILuint w,h;

        w = ilGetInteger(IL_IMAGE_WIDTH);
        h = ilGetInteger(IL_IMAGE_HEIGHT);

        cout << "Specular map loaded: " << _ks_map << "\n";

        textures.push_back(new Texture(w,h,ilGetData()));
        ks_map = textures.size()-1;
      } else {
        //Image not loaded (?)
        cout << "Specular map not loaded error code: " << ilGetError() << endl;
      }
    }

    //Reflection map
    if(!_r_map.empty()) {
      image = ilGenImage();
      ilBindImage(image);

      ilLoadImage(_r_map.c_str());

      if(ilGetError() == IL_NO_ERROR) {
        ILuint w,h;

        w = ilGetInteger(IL_IMAGE_WIDTH);
        h = ilGetInteger(IL_IMAGE_HEIGHT);

        cout << "Reflection map loaded: " << _r_map << "\n";

        textures.push_back(new Texture(w,h,ilGetData()));
        r_map = textures.size()-1;
      } else {
        //Image not loaded (?)
        cout << "Reflection map not loaded error code: " << ilGetError() << endl;
      }
    }

    //Transparency map
    if(!_d_map.empty()) {
      image = ilGenImage();
      ilBindImage(image);

      ilLoadImage(_d_map.c_str());

      if(ilGetError() == IL_NO_ERROR) {
        ILuint w,h;

        w = ilGetInteger(IL_IMAGE_WIDTH);
        h = ilGetInteger(IL_IMAGE_HEIGHT);

        textures.push_back(new Texture(w,h,ilGetData()));
        d_map = textures.size()-1;
      } else {
        //Image not loaded (?)
        cout << "Transparency map not loaded" << endl;
      }
    }

		OBJImporter::materials.push_back(new Material(
		    _name,
		    _ambient,
		    _diffuse,
		    _specular,
		    _emissive,
				_transparency,
				_shininess,
				_sharpness,
				_reflection,
				_index_of_refraction,
				fresnel,
				diff_map_index,
				bump_map,
				norm_map,
				ks_map,
				r_map,
				d_map,
				_reflect_spread,
				_reflect_samples,
				_refract_spread,
				_refract_samples,
				_projector,
				_corresponder,
				_scale,
				use_relief,
				_axis,
				use_position));
	}

  float inf=std::numeric_limits<float>::infinity();
  float min[3]={inf,inf,inf};
  float max[3]={-inf,-inf,-inf};
	// Start creating the triangles
	for(int i=0; i<obj_data->faceCount; i++){
		obj_face *face = obj_data->faceList[i];

		vector<vec3*> _vertices;
		vector<vec3*> _normals;
		vector<vec3*> _texCoords;
		for(int j=0;j<3;j++){
			obj_vector* _vec  = obj_data->vertexList[ face->vertex_index[j] ];
			obj_vector* _norm = obj_data->normalList[ face->normal_index[j] ];

			obj_vector* _text = NULL;

			if(face->texture_index[j] != -1)
			  _text = obj_data->textureList[ face->texture_index[j] ];

			_vertices.push_back(new vec3(_vec->e[0],_vec->e[1],_vec->e[2]));
			_normals.push_back(new vec3(_norm->e[0],_norm->e[1],_norm->e[2]));

		  for (int t = 0; t < 3; t++) {
        testMax(&max[t], _vec->e[t]);
        testMin(&min[t], _vec->e[t]);
      } // IN BOTH

			if(face->texture_index[j] != -1)
			  _texCoords.push_back(new vec3(_text->e[0],_text->e[1],_text->e[2]));
			else _texCoords.push_back(new vec3(0,0,0));
		}
		unsigned int _material = face->material_index+materialOffset;
		OBJImporter::triangles.push_back(new Triangle(_vertices,_normals,_texCoords,_material));

		// More than one triangle for each face?
		for(int f=0;f<face->vertex_count%3;f++){
			vector<vec3*> _vertices;
			vector<vec3*> _normals;
			vector<vec3*> _textures;
			for(int j=0;j<4;j++){
				if(j!=(f+1)){
						obj_vector* _vec  = obj_data->vertexList[ face->vertex_index[j] ];
						obj_vector* _norm = obj_data->normalList[ face->normal_index[j] ];

						obj_vector* _text;

						if(face->texture_index[j] != -1)
						  _text = obj_data->textureList[ face->texture_index[j] ];

						_vertices.push_back(new vec3(_vec->e[0],_vec->e[1],_vec->e[2]));
						_normals.push_back(new vec3(_norm->e[0],_norm->e[1],_norm->e[2]));


						for (int t = 0; t < 3; t++) {
						  testMax(&max[t], _vec->e[t]);
						  testMin(&min[t], _vec->e[t]);
						} // IN BOTH

						if(face->texture_index[j] != -1)
						  _textures.push_back(new vec3(_text->e[0],_text->e[1],_text->e[2]));
						else _texCoords.push_back(new vec3(0,0,0));
				}
			}
			OBJImporter::triangles.push_back(new Triangle(_vertices,_normals,_texCoords,_material));
		}

	}

	delete(obj_data);
  aabb = AABB(min[0],min[1],min[2],max[0]-min[0],max[1]-min[1],max[2]-min[2]);
}
void OBJImporter::testMax(float* max,float value){
  if(value>*(max))
    *(max)=value;
}
void OBJImporter::testMin(float* min,float value){
  if(value<*(min))
      *(min)=value;
}

std::vector<Triangle*>& OBJImporter::getTriangleList(){
	return OBJImporter::triangles;
}
std::vector<Material*>& OBJImporter::getMaterialList(){
	return materials;
}
std::vector<Texture*>& OBJImporter::getTextures() {
  return textures;
}

AABB OBJImporter::getAABB(){
  return aabb;
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


