/*
 * Scene.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Scene.h"
#include "../AccDataStructImpl/ArrayDataStruct.h"

namespace raytracer {

Scene::Scene()
  : m_camera(), m_lights(), m_materials() {
  m_acc_data_struct = new ArrayDataStruct();
}

Scene::~Scene() {}

void Scene::loadTriangles(Triangle* triangles, int length, bool overwrite) {
  m_acc_data_struct->addData(triangles, length);
}

void Scene::loadCamera(Camera camera) {
  m_camera = camera;
}

void Scene::loadLights(ILight* lights, int length, bool overwrite) {
  for (size_t i; i<length; ++i) {
    m_lights.push_back(&lights[i]);
  }
}

void Scene::loadMaterials(Material* materials, int length) {
  for (size_t i; i<length; ++i) {
    m_materials.push_back(&materials[i]);
  }
}

const std::vector<ILight*>& Scene::getLightVector() {
  return m_lights;
}

const std::vector<Material*>& Scene::getMaterialVector() {
  return m_materials;
}

const Camera& Scene::getCamera() {
  return m_camera;
}

IAccDataStruct* Scene::getAccDataStruct() {
  return m_acc_data_struct;
}


}
