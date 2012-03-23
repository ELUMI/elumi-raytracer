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

#include "XMLImpl/XML.h"

#include "IPostEffect.h"
#include "PostEffectImpl/ReinhardOperator.h"
#include "PostEffectImpl/ClampOperator.h"
#include "PostEffectImpl/GammaEncode.h"

#include "TracerImpl/PhotonMapper.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

namespace raytracer {

Renderer::Renderer(int open_gl_version) {
  this->open_gl_version = open_gl_version;

  Settings* set = new Settings();
  set->opengl_version = open_gl_version;

  m_scene = new Scene(set);

  m_tracer = NULL;
  color_buffer = NULL;
  init();
}

//Renderer::Renderer(Settings* settings) {
//  m_scene = new Scene(settings);
//  m_settings = settings;
//  init();
//}



//Renderer::Renderer(Scene* scene) {
//  m_scene = scene;
//  init();
//}

Renderer::~Renderer() {
  delete m_tracer;
  delete [] color_buffer;
  delete m_scene;
}

void Renderer::init() {

  if(m_tracer != NULL)
      delete m_tracer;
  if(color_buffer != NULL)
      delete [] color_buffer;

  switch (m_scene->getSettings()->tracer) {
  case -1:
    m_tracer = new DebugTracer(m_scene);
    break;
  case 0:
    m_tracer = new BaseTracer(m_scene);
    break;
  case 1:
    m_tracer = new SimpleTracer(m_scene);
    break;
  case 2:
  default:
    m_tracer = new StandardTracer(m_scene);
    break;
  }
  buffer_length = m_scene->getSettings()->width * m_scene->getSettings()->height * 4;
  color_buffer = new float[buffer_length];
  renderthread = 0;
}

void Renderer::loadTriangles(vector<Triangle*> triangles, bool overwrite) {
  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }
  m_scene->loadTriangles(triangles, overwrite);
}

void Renderer::loadCamera(Camera& camera) {
  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }
  m_scene->loadCamera(camera);
}

void Renderer::loadLights(ILight** lights, int length, bool overwrite) {
  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }
  m_scene->loadLights(lights,length,overwrite);
}

void Renderer::loadSceneFromXML(const char* filename) {
  XML xml = XML(open_gl_version);
  if(m_scene != NULL)
    delete m_scene;
  m_scene = xml.importScene(filename);
  init();
}

Scene* Renderer::getScene() {
  return m_scene;
}

ITracer* Renderer::getTracer() {
  return m_tracer;
}

float* Renderer::getColorBuffer() {
  return color_buffer;
}

void Renderer::setSettings(Settings* settings) {
  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }
  m_scene->setSettings(settings);
}

void Renderer::asyncRender() {
  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }
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

  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }

  m_tracer->traceImage(color_buffer);

  ReinhardOperator reinhard = ReinhardOperator();
  GammaEncode gamma = GammaEncode();
  ClampOperator clamp = ClampOperator();

  reinhard.run(color_buffer,buffer_length);
  //gamma.run(color_buffer,buffer_length);
  clamp.run(color_buffer,buffer_length);

}

unsigned int Renderer::renderComplete() {
  if(m_tracer == NULL) {
    cout << "Render has no tracer!\n";
    return -1;
  }
  return m_tracer->getPixelsLeft();
}

}
