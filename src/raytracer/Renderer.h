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
#include "glm/glm.hpp"


using namespace glm;

namespace raytracer {

class Renderer {
public:
  Renderer(int open_gl_version);
  virtual ~Renderer();
  
  Scene* getScene();
  ITracer* getTracer();

  void loadSceneFromXML(const char* filename);
  void loadTriangles(vector<Triangle*> triangles,AABB* aabb, bool overwrite=false);
  void loadCamera(Camera& camera);
  void loadLights(ILight** lights, int length, bool overwrite=false);
  void setSettings(Settings* settings);


  float* getColorBuffer();
  void render();       // synchronic
  void asyncRender();  // asynchronic
  void stopRendering();// synchronic
  void tonemapImage(bool enable);
  
  /*
   * @return The number of pixels left to render
   */
  float renderComplete();
  
private:
  void init();
  Scene* m_scene;
  size_t buffer_length;
  ITracer* m_tracer;
  bool abort;
  int open_gl_version;
  float* color_buffer_other;
  float* color_buffer;

  bool tonemapped;
  boost::thread* renderthread;
};

}

#endif /* RENDERER_H_ */
