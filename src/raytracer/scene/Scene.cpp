/*
 * Scene.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Scene.h"
#include "../AccDataStructImpl/ArrayDataStruct.h"

namespace raytracer {

Scene::Scene(Settings* settings)
  : m_camera(), m_lights(), m_materials() {
  m_acc_data_struct = new ArrayDataStruct();
  m_settings = settings;
}

Scene::~Scene() {}

void Scene::loadTriangles(vector<Triangle*> triangles, bool overwrite) {
  m_acc_data_struct->setData(triangles);
  if(m_settings->opengl_version){
    m_drawable = new VertexArrayDataStruct(this, triangles);
  }
}

void Scene::loadCamera(Camera camera) {
  m_camera = camera;
}

void Scene::loadLights(ILight* lights, int length, bool overwrite) {
  for (int i=0; i<length; ++i) {
    m_lights.push_back(&lights[i]);
  }
}

void Scene::loadMaterials(Material* materials, int length) {
  for (size_t i; i<length; ++i) {
    m_materials.push_back(&materials[i]);
  }
}

void Scene::loadMaterials(std::vector<raytracer::Material*> materials) {
  m_materials = materials;
}
//Adds a texture and returns its index
int Scene::addTexture(Texture* texture) {
  m_textures.push_back(texture);
  return m_textures.size()-1;
}

const std::vector<ILight*>& Scene::getLightVector() {
  return m_lights;
}

const std::vector<Material*>& Scene::getMaterialVector() {
  return m_materials;
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

}
