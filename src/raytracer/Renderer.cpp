/*
 * Renderer.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Renderer.h"

namespace raytracer {

Renderer::Renderer()
  : m_scene() {

}

Renderer::~Renderer() {

}

void Renderer::loadTriangles(Triangle* triangles, int length, bool overwrite) {

}

void Renderer::loadCamera(Camera& camera) {

}

void Renderer::loadLights(ILight* lights, int length, bool overwrite) {
  m_scene.loadLights(lights,length,overwrite);
}

void Renderer::loadSettings(Settings& settings) {
	*m_settings = settings;
}

uint8_t* Renderer::getFloatArray() {
	return 0;
}

void Renderer::render() {

}

int Renderer::renderComplete() {

	return -1;
}

}
