/*
 * Renderer.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Renderer.h"
#include "TracerImpl/SimpleTracer.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

namespace raytracer {

Renderer::Renderer(Settings* settings)
  : m_scene(settings) {
  m_settings = settings;
  m_tracer = new SimpleTracer(&m_scene, settings->backgroundColor);
  color_buffer = new uint8_t[m_settings->width * m_settings->height * 4];//(uint8_t *) calloc (sizeof (uint8_t), m_settings->width * m_settings->height * 4);
  renderthread = 0;
}

Renderer::~Renderer() {
  stopRendering();
  delete m_tracer;
  delete [] color_buffer;
}

void Renderer::loadTriangles(vector<Triangle*> triangles, bool overwrite) {
  m_scene.loadTriangles(triangles, overwrite);
}

void Renderer::loadCamera(Camera& camera) {
  m_scene.loadCamera(camera);
}

void Renderer::loadLights(ILight* lights, int length, bool overwrite) {
  m_scene.loadLights(lights,length,overwrite);
}

Scene& Renderer::getScene() {
  return m_scene;
}

ITracer* Renderer::getTracer() {
  return m_tracer;
}

uint8_t* Renderer::getFloatArray() {
	return color_buffer;
}

void Renderer::asyncRender() {
  m_tracer->first_bounce(m_settings->width*m_settings->height,color_buffer); //must be done in master thread

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
  int rays_length = m_settings->width*m_settings->height;

  //Initiate ray array
  Ray* rays = new Ray[rays_length];

  Camera camera = m_scene.getCamera();
  mat4 view = camera.getViewMatrix();

  float w = m_settings->width,
        h = m_settings->height;
  mat4 viewport(w/2,  0.0f, 0.0f, w/2,
                0.0f, h/2,  0.0f, h/2,
                0.0f, 0.0f, 0.5f, 0.5f,
                0.0f, 0.0f, 0.0f, 1.0f);

  mat4 trans = inverse(view) * transpose(inverse(viewport));

  //We step over all "pixels" from the cameras viewpoint
  for(int y = 0; y < m_settings->height; y++) {
    for(int x = 0; x < m_settings->width; x++) {
      vec4 dir = trans * vec4(x,y,1,1);

      rays[x+y*m_settings->width] = Ray(camera.getPosition(), vec3(normalize(dir)));
    }
  }

  m_tracer->trace(rays,m_settings->width*m_settings->height,color_buffer);

  //Free memory
  delete [] rays;
}

unsigned int Renderer::renderComplete() {
	return m_tracer->getPixelsLeft();
}

}
