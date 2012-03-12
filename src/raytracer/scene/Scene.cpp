/*
 * Scene.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Scene.h"
#include "../AccDataStructImpl/ArrayDataStruct.h"
#include "../AccDataStructImpl/TriangleArray.h"

namespace raytracer {

Scene::Scene(Settings* settings)
  : m_camera(), m_lights(), m_materials() {
  m_acc_data_struct = new ArrayDataStruct();
  m_settings = settings;
}

Scene::~Scene() {}

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
void Scene::loadTextures(std::vector<raytracer::Texture*> textures) {
  m_textures = textures;
}

const std::vector<ILight*>& Scene::getLightVector() {
  return m_lights;
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

}
