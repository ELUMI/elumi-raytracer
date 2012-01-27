/*
 * Renderer.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Renderer.h"
#include "TracerImpl/SimpleTracer.h"

namespace raytracer {

Renderer::Renderer()
  : m_scene() {
  //m_settings =
  m_tracer = new SimpleTracer(&m_scene);
}

Renderer::~Renderer() {
  delete m_tracer;
}

void Renderer::loadTriangles(Triangle* triangles, int length, bool overwrite) {
  m_scene.loadTriangles(triangles, length, overwrite);
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
  // Create rays and send to tracer
  //m_tracer->trace(rays);

}

int Renderer::renderComplete() {

	return -1;
}

}
