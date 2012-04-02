/*
 * Renderer.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Renderer.h"
#include "TracerImpl/DebugTracer.h"
#include "TracerImpl/StandardTracer.h"
#include "TracerImpl/PhotonMapper.h"
#include "TracerImpl/AdvancedTracer.h"
#include "TracerImpl/PathTracer.h"

#include "XMLImpl/XML.h"

#include "IPostEffect.h"
#include "PostEffectImpl/ReinhardOperator.h"
#include "PostEffectImpl/ClampOperator.h"
#include "PostEffectImpl/GammaEncode.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;

namespace raytracer {

Renderer::Renderer(int open_gl_version):color_buffer_org(NULL) {
  this->open_gl_version = open_gl_version;
  abort = false;

  Settings* set = new Settings();
  set->opengl_version = open_gl_version;

  m_scene = new Scene(set);

  m_tracer = NULL;
  color_buffer = NULL;
  init();
}

Renderer::~Renderer() {
  stopRendering();
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
    cout << "Using debug tracer\n";
    break;
  case 0:
    m_tracer = new BaseTracer(m_scene);
    cout << "Using base tracer\n";
    break;
  case 2:
  default:
    m_tracer = new StandardTracer(m_scene);
    cout << "Using standard tracer\n";
    break;
  case 3:
    m_tracer = new PhotonMapper(m_scene);
    cout << "Using photon mapper tracer\n";
    break;
  case 4:
    m_tracer = new AdvancedTracer(m_scene);
    cout << "Using advanced tracer\n";
    break;
  case 5:
    m_tracer = new PathTracer(m_scene);
    cout << "Using path tracer\n";
    break;
  }
  buffer_length = m_scene->getSettings()->width * m_scene->getSettings()->height * 4;
  color_buffer = new float[buffer_length];
  renderthread = 0;
}


void Renderer::loadTriangles(vector<Triangle*> triangles,AABB* aabb, bool overwrite) {
  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }
  m_scene->loadTriangles(triangles,aabb, overwrite);
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
    abort = true;
    if(m_tracer)
      m_tracer->stopTracing();
    renderthread->join();
    delete renderthread;
    renderthread = 0;
  }
}

void Renderer::tonemapImage(bool enable){
/*
  if(color_buffer_other==NULL){
    Settings* set = m_scene->getSettings();
    ReinhardOperator reinhard = ReinhardOperator(set->key, set->white);
    GammaEncode gamma = GammaEncode();
    ClampOperator clamp = ClampOperator();

    color_buffer_other = new float[buffer_length];
    for(size_t s_buffer=0;s_buffer<buffer_length;s_buffer++){
      color_buffer_other[s_buffer] = color_buffer[s_buffer];
    }

    reinhard.run(color_buffer_other,buffer_length / 4, 4);

    clamp.run(color_buffer_other,buffer_length / 4, 4);

  }

  if ((enable && !tonemapped) || (!enable && tonemapped)) {
    tonemapped=!tonemapped;
    float* color_buffer_temp= new float[buffer_length];;
    for(size_t s_buffer=0;s_buffer<buffer_length;s_buffer++){
      color_buffer_temp[s_buffer] = color_buffer[s_buffer];
    }
    for(size_t s_buffer=0;s_buffer<buffer_length;s_buffer++){
      color_buffer[s_buffer] = color_buffer_other[s_buffer];
      color_buffer_other[s_buffer] = color_buffer_temp[s_buffer];
    }
    delete[] color_buffer_temp;
  }
*/
  if(color_buffer_org==NULL){
    color_buffer_org = new float[buffer_length];
    for (size_t s_buffer = 0; s_buffer < buffer_length; s_buffer++) {
      color_buffer_org[s_buffer] = color_buffer[s_buffer];
    }
  }

  if (enable) {
    Settings* set = m_scene->getSettings();
    ReinhardOperator reinhard = ReinhardOperator(set->key, set->white);
    GammaEncode gamma = GammaEncode();
    ClampOperator clamp = ClampOperator();

    for (size_t s_buffer = 0; s_buffer < buffer_length; s_buffer++) {
      color_buffer[s_buffer] = color_buffer_org[s_buffer];
    }

    reinhard.run(color_buffer, buffer_length / 4, 4);
    clamp.run(color_buffer, buffer_length / 4, 4);
  } else {
    for (size_t s_buffer = 0; s_buffer < buffer_length; s_buffer++) {
      color_buffer[s_buffer] = color_buffer_org[s_buffer];
    }
  }
}

void Renderer::render() {
  abort = false;
  tonemapped = false;
  if(color_buffer_org!=NULL){
    delete[] color_buffer_org;
    color_buffer_org = NULL;
  }

  if(m_scene == NULL) {
    cout << "Render has no scene!\n";
    return;
  }

  m_tracer->traceImage(color_buffer);

  if(abort)
    return;
}

float Renderer::renderComplete() {
  if(m_tracer == NULL) {
    cout << "Render has no tracer!\n";
    return -1;
  }
  return m_tracer->getPixelsLeft();
}

}
