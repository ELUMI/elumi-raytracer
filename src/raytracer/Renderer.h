/*
 * Renderer.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "Settings.h"
#include "ITracer.h"
#include "scene/Scene.h"
#include "utilities/Triangle.h"

#include <iostream>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>

#include "glm/glm.hpp"
using namespace glm;

namespace raytracer {

class Renderer {
public:
  Renderer(int open_gl_version);
  virtual ~Renderer();
  
  Scene* getScene();
  ITracer* getTracer();

  void loadSceneFromXML(const char* filename, const char* settingsFileName);
  void loadTriangles(vector<Triangle*> triangles,AABB aabb, bool overwrite=false);
  void loadCamera(Camera& camera);
  void loadLights(ILight** lights, int length, bool overwrite=false);
  void setSettings(Settings* settings);


  float* getColorBuffer();
  void render();       // synchronic
  void asyncRender();  // asynchronic
  void stopRendering();// synchronic
  void tonemapImage();
  
  float renderComplete();
  
private:
  void init();
  Scene* m_scene;
  size_t buffer_length;
  ITracer* m_tracer;
  bool abort;
  int open_gl_version;
  float* color_buffer_org;
  float* color_buffer;
  void doRender();

  bool tonemapped;
  boost::thread* renderthread;
  bool initing;

  boost::timer::cpu_timer timer;
};

}

#endif /* RENDERER_H_ */
