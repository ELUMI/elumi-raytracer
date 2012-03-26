/*
 * Scene.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Scene.h"
#include "../AccDataStructImpl/VertexArrayDataStruct.h"
#include "../AccDataStructImpl/ArrayDataStruct.h"
#include "../AccDataStructImpl/TriangleArray.h"
#include "../AccDataStructImpl/TestHeightMapDataStruct.h"

namespace raytracer {


Scene::Scene(Settings* settings)
  : m_camera(), m_materials() {
  m_lights = new std::vector<ILight*>;
  m_acc_data_struct = new TestHeightMapDataStruct();
  m_settings = settings;
}


Scene::~Scene() {
  // TODO AAAAASMYCKET SKRÄPHANTERING FRÅN ALLTING!!!!
  delete m_lights;
  delete m_settings;
  delete m_acc_data_struct;
}

void Scene::loadTriangles(vector<Triangle*> triangles, bool overwrite) {
  m_acc_data_struct->setData(triangles);
  if(m_settings->opengl_version == 3){
    m_drawable = new VertexArrayDataStruct(this, triangles);
  } else if(m_settings->opengl_version == 2){
    m_drawable = new TriangleArray(this, triangles);
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

void Scene::loadMaterials(Material* materials, size_t length) {
  for (size_t i; i<length; ++i) {
    m_materials.push_back(&materials[i]);
  }
}

void Scene::loadMaterials(std::vector<raytracer::Material*> materials) {
  m_materials = materials;
}


void Scene::loadTextures(std::vector<raytracer::Texture*> textures) {
  for(int i = 0; i < textures.size();i++) {
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

}
