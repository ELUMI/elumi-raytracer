/*
 * Scene.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Scene.h"
#include "../AccDataStructImpl/ArrayDataStruct.h"
#include "../AccDataStructImpl/KDTreeDataStruct.hpp"
#include "../AccDataStructImpl/HashDataStruct.h"
#include "../AccDataStructImpl/TestHeightMapDataStruct.h"
#include "../GLData/VertexArrayDataStruct.h"
#include "../GLData/TriangleArray.h"

namespace raytracer {


Scene::Scene(Settings* settings)
: m_camera(), m_materials(), aabb(vec3(),vec3()) {
  m_lights = new std::vector<ILight*>;
  switch(settings->tree){
  case 1:
    m_acc_data_struct = new ArrayDataStruct();
    break;
  case 2:
  default:
    m_acc_data_struct = new KDTreeDataStruct(settings);
    break;
  case 3:
    m_acc_data_struct = new HashDataStruct(0.1f, 1024);
    break;
  }
  m_settings = settings;
  m_drawable = NULL;
  environment_map = NULL;
}


Scene::~Scene() {
  // Deletes all scene data
  delete m_settings;

  for(size_t i=0; i<m_lights->size(); ++i)
    delete m_lights->at(i);
  delete m_lights;

  for(size_t i=0; i<m_textures.size(); ++i)
    delete m_textures[i];

  for(size_t i=0; i<m_materials.size(); ++i)
    delete m_materials[i];

  for(size_t i=0; i<m_triangles.size(); ++i)
    delete m_triangles[i];

  delete m_acc_data_struct;

  if (m_drawable!=NULL)
    delete m_drawable;

  if (environment_map!=NULL)
    delete environment_map;

  ilShutDown();
}

void Scene::loadTriangles(vector<Triangle*> triangles, AABB aabb, size_t material_shift) {
  this->aabb.extend(aabb);
  if(material_shift) {
    for(size_t i=0;i<triangles.size(); ++i){
      triangles[i]->addMaterialShift(material_shift);
    }
  }
  m_triangles.insert(m_triangles.end(), triangles.begin(), triangles.end());
}

void Scene::build() {
  m_acc_data_struct->setData(m_triangles.data(),m_triangles.size(),aabb);

  if (m_drawable!=NULL)
    delete m_drawable;
  if(m_settings->opengl_version == 3){
    m_drawable = new VertexArrayDataStruct(this, m_triangles);
  } else if(m_settings->opengl_version == 2){
    m_drawable = new TriangleArray(this, m_triangles);
  }
}

void Scene::loadCamera(Camera camera) {
  m_camera = camera;
}

void Scene::loadLights(ILight** lights, size_t length, bool overwrite) {
  for (size_t i=0; i<length; ++i) {
    m_lights->push_back(lights[i]);
  }
}

size_t Scene::loadMaterials(Material* materials, size_t length) {
  size_t size = m_materials.size();
  for (size_t i; i<length; ++i) {
    m_materials.push_back(&materials[i]);
  }
  return size;
}

size_t Scene::loadMaterials(std::vector<raytracer::Material*> materials) {
  size_t size = m_materials.size();
  m_materials.insert(m_materials.end(), materials.begin(), materials.end());
  return size;
}


void Scene::loadTextures(std::vector<raytracer::Texture*> textures) {
  for(size_t i = 0; i < textures.size();i++) {
    m_textures.push_back(textures.at(i));
  }
}

void Scene::setSettings(Settings* settings) {
  m_settings = settings;
}

std::vector<ILight*>* Scene::getLightVector() {
  return m_lights;
}

const std::vector<Texture*>& Scene::getTextures() {
  return m_textures;
}

const std::vector<Material*>& Scene::getMaterialVector() {
  return m_materials;
}

Texture* Scene::getTextureAt(int index) {
  return m_textures.at(index);
}

Camera& Scene::getCamera() {
  return m_camera;
}

IAccDataStruct* Scene::getAccDataStruct() {
  return m_acc_data_struct;
}

IDraw* Scene::getDrawable(){
  return m_drawable;
}

Settings* Scene::getSettings() {
  return m_settings;
}

IEnvironmentMap* Scene::getEnvironmentMap() {
  return environment_map;
}

void Scene::setEnvirontmentMap(IEnvironmentMap* environment_map) {
  this->environment_map = environment_map;
}

}
