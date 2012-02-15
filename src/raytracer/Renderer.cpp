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
  : m_scene() {
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

uint8_t* Renderer::getFloatArray() {
	return color_buffer;
}

void Renderer::asyncRender() {
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
  view = inverse(view);

  //We step over all "pixels" from the cameras viewpoint
  for(int h = 0; h < m_settings->height; h++) {
    for(int w = 0; w < m_settings->width; w++) {
      vec4 dir = vec4(2.0f*float(w)/(m_settings->width -1)-1.0f,
                      2.0f*float(h)/(m_settings->height-1)-1.0f, 1.0f, 1.0f);
      dir = view*dir;

      rays[w+h*m_settings->width] = Ray(camera.getPosition(),vec3(dir));

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
