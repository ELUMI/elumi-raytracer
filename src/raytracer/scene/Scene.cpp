/*
 * Scene.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Scene.h"

namespace raytracer {

Scene::Scene()
  : m_lights(), m_materials() {



}

Scene::~Scene() {

}

void Scene::loadTriangles(Triangle* triangles, int length, bool overwrite) {
  //m_AccDataStruct.addData(triangles, length);
}

void Scene::loadLights(ILight* lights, int length, bool overwrite) {
  for (size_t i; i<length; ++i) {
    m_lights.push_back(lights[i]);
  }
}

}
