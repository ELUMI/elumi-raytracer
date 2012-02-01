/*
 * Renderer.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Renderer.h"
#include "TracerImpl/SimpleTracer.h"

using namespace std;

namespace raytracer {

Renderer::Renderer(Settings* settings)
  : m_scene() {
  m_settings = settings;
  m_tracer = new SimpleTracer(&m_scene, settings->backgroundColor);
  color_buffer = new uint8_t[m_settings->width * m_settings->height * 4];//(uint8_t *) calloc (sizeof (uint8_t), m_settings->width * m_settings->height * 4);
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

void Renderer::loadLights(ILight* lights, int length, bool overwrite) {
  m_scene.loadLights(lights,length,overwrite);
}

uint8_t* Renderer::getFloatArray() {
	return color_buffer;
}

void Renderer::render() {
  int rays_length = m_settings->width*m_settings->height;

  //Initiate ray array
  Ray* rays = new Ray[rays_length];

  Camera camera = m_scene.getCamera();

  //Create an up vector and align
  vec3 up_vector = camera.getUpVector() * tan(camera.getFov()/2) * 2.0f * (1/camera.getAspectRatio());

  //Create an horizontal vector
  vec3 horizontal_vector = cross(camera.getUpVector(), camera.getDirection()) * tan(camera.getFov()/2) * 2.0f;

  //Create a vector for stepping vertically
  vec3 step_h_vector = up_vector * (1/((float)m_settings->height));

  //Create a vector for stepping horizontally
  vec3 step_w_vector = horizontal_vector * (1/((float)m_settings->width));

  //Create a pixel vector and align it
  vec3 pixel_vector = camera.getDirection() + up_vector * 0.5f + horizontal_vector * 0.5f;

  //We step over all "pixels" from the cameras viewpoint
  for(int h = 0; h < m_settings->height; h++) {
    for(int w = 0; w < m_settings->width; w++) {
      //Create a ray for this pixel
      rays[w+h*m_settings->width] = Ray(camera.getPosition(),pixel_vector);
      //Step along the x-axis
      //cout << "x: " << pixel_vector.x << " y: " << pixel_vector.y << " z: " << pixel_vector.z << endl;

      pixel_vector -= step_w_vector;
    }
    //Reset pixel vector horizontally
    pixel_vector += horizontal_vector;
    //Step along the y-axis
    pixel_vector -= step_h_vector;
  }

  m_tracer->trace(rays,m_settings->width*m_settings->height,color_buffer);

  //Free memory
  delete [] rays;

}

int Renderer::renderComplete() {

	return -1;
}

}
