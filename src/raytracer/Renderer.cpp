/*
 * Renderer.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Renderer.h"
#include "TracerImpl/DebugTracer.h"
#include "TracerImpl/SimpleTracer.h"
#include "TracerImpl/StandardTracer.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

namespace raytracer {

Renderer::Renderer(Settings* settings)
  : m_scene(settings) {
  m_settings = settings;
  switch (settings->tracer) {
    case -1:
      m_tracer = new DebugTracer(&m_scene, settings);
      break;
    case 0:
      m_tracer = new BaseTracer(&m_scene, settings);
      break;
    case 1:
      m_tracer = new SimpleTracer(&m_scene, settings);
      break;
    case 2:
    default:
      m_tracer = new StandardTracer(&m_scene, settings);
      break;
  }
  color_buffer = new float[m_settings->width * m_settings->height * 4];
  renderthread = 0;
}

Renderer::~Renderer() {
  delete m_tracer;
  delete [] color_buffer;
}

void Renderer::loadTriangles(vector<Triangle*> triangles, bool overwrite) {
  m_scene.loadTriangles(triangles, overwrite);
}

void Renderer::loadCamera(Camera& camera) {
  m_scene.loadCamera(camera);
}

void Renderer::loadLights(ILight** lights, int length, bool overwrite) {
  m_scene.loadLights(lights,length,overwrite);
}

Scene& Renderer::getScene() {
  return m_scene;
}

ITracer* Renderer::getTracer() {
  return m_tracer;
}

float* Renderer::getColorBuffer() {
	return color_buffer;
}

void Renderer::asyncRender() {
  m_tracer->first_bounce(); //must be done in master thread

  if(renderthread){
    stopRendering();
  }
  renderthread = new boost::thread( boost::bind(&Renderer::render, this ));
}

void Renderer::stopRendering() {
  if(renderthread) {
    if(m_tracer)
      m_tracer->stopTracing();
    renderthread->join();
    delete renderthread;
    renderthread = 0;
  }
}


void Renderer::render() {
  m_tracer->traceImage(color_buffer);

  // POST FXS!
}

unsigned int Renderer::renderComplete() {
	return m_tracer->getPixelsLeft();
}

}
