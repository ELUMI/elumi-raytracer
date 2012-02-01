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

Renderer::Renderer(Settings& settings)
  : m_scene() {
  *m_settings = settings;
  m_tracer = new SimpleTracer(&m_scene);
  color_buffer = (uint8_t *) calloc (sizeof (uint8_t), m_settings->width * m_settings->height * 4);
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

uint8_t* Renderer::getFloatArray() {
	return 0;
}

void Renderer::render() {
  // Create rays and send to tracer
  //m_tracer->trace(rays);

  int rays_length = m_settings->width*m_settings->height;

  //Initiate ray array
  Ray* rays = new Ray[rays_length];

  Camera camera = m_scene.getCamera();

  //Create an up vector and align
  vec3 up_vector = camera.getNormal() * camera.getFov() * (1/camera.getAspectRatio());

  //Create an horizontal vector
  vec3 horizontal_vector = cross(camera.getNormal(), camera.getDirection()) * camera.getFov();

  //Create a vector for stepping vertically
  vec3 step_h_vector = up_vector * (1/((float)m_settings->height));

  //Create a vector for stepping horizontally
  vec3 step_w_vector = horizontal_vector * (1/((float)m_settings->width));

  //Create a pixel vector and align it
  vec3 pixel_vector = camera.getDirection() + up_vector * 0.5f + horizontal_vector * 0.5f;

  //We step over all "pixels" across the FOV and aspect ratio
  for(int h = 0; h < m_settings->height; h++) {
    for(int w = 0; w < m_settings->width; w++) {
      //Create a ray for this pixel
      rays[w+h*m_settings->width] = Ray(camera.getPosition(),pixel_vector);
      //Step along the x-axis
      pixel_vector -= step_w_vector;
    }
    //Reset pixel vector horizontally
    pixel_vector += 2.0f*horizontal_vector;
    //Step along the y-axis
    pixel_vector -= step_h_vector;
  }

  //m_tracer.trace(rays,m_settings->width*m_settings->height,color_buffer);

  //Free memory
  delete [] rays;

}

int Renderer::renderComplete() {

	return -1;
}

}
