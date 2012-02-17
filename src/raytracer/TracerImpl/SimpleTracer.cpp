/*
 * SimpleTracer.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: david
 */

#include <iostream>

#include "SimpleTracer.h"
#include <stdint.h>

#include <glm/glm.hpp>

using namespace glm;

namespace raytracer {

//ITracer::~ITracer() {}

SimpleTracer::SimpleTracer(Scene* scene, vec4 background_color) { //TODO: get from settings!!!
  SimpleTracer::scene = scene;
  SimpleTracer::background_color = background_color;
  abort = false;
 }

SimpleTracer::~SimpleTracer() {
  stopTracing();
}

void SimpleTracer::stopTracing() {
  abort = true;
}

unsigned int SimpleTracer::getPixelsLeft(){
  return pixelsLeft;
}

void SimpleTracer::first_bounce(int length, uint8_t* buffer) {
  //first_pass.render(scene, scene->getCamera().getViewMatrix());
}

void SimpleTracer::trace(Ray* rays, int length, uint8_t* buffer) {
  abort = false;
  SimpleTracer::buffer = buffer;
  pixelsLeft = length;

  #pragma omp parallel for
  for (size_t i=0; i<length; ++i) {
    //#pragma omp task
    #pragma omp flush (abort)
    if(!abort)
    {
      vec4 c = traceHelper(&rays[i]);
      buffer[i*4] = 255*glm::min(1.0f, c.r);
      buffer[i*4 +1] = 255*glm::min(1.0f, c.g);
      buffer[i*4 +2] = 255*glm::min(1.0f, c.b);
      buffer[i*4 +3] = 255*glm::min(1.0f, c.a);

      #pragma omp atomic
      --pixelsLeft;
    }
  }
}

vec4 SimpleTracer::traceHelper(Ray* ray, int levels) {
  vec3 color_black = vec3(0.0f,0.0f,0.0f);

  IAccDataStruct::IntersectionData intersection_data = scene->getAccDataStruct()->findClosestIntersection(*ray);
  if (intersection_data.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    return background_color;

  } else {
    Material* material = scene->getMaterialVector()[intersection_data.material];

    Triangle* triangle = intersection_data.triangle;

    vec3 texture_color = vec3(0,0,0);

    if(material->getTexture() != -1) {

      Texture* texture = scene->getTextureAt(material->getTexture());

      vec3 p = intersection_data.interPoint;

      float x = p.x;
      float y = p.y;

      if(x > y) {
        x = p.y;
        y = p.x;
      }
      if(y > p.z) {
        y = p.z;
      }

      vec2 texCoords = vec2(x,y);

      texCoords.x = (int)(texCoords.x*(texture->getWidth()/2))%texture->getWidth();
      texCoords.y = (int)(texCoords.y*(texture->getHeight()/2))%texture->getHeight();

      int x_coord = (int)texCoords.x;
      int y_coord = (int)texCoords.y;

      texture_color = texture->getColorAt(x_coord,y_coord)/255.0f;
    }

    vec3 color;
    //Light
    ILight* light = scene->getLightVector().front();
    vec3 dir = normalize(intersection_data.interPoint - light->getPosition());
    Ray lightRay = Ray(light->getPosition(), dir);
    IAccDataStruct::IntersectionData intersection_data_light = scene->getAccDataStruct()->findClosestIntersection(lightRay);

    if (//intersection_data_light.triangle == intersection_data.triangle ||
        dot(dir,(intersection_data.interPoint - intersection_data_light.interPoint)) > 0.001) {
      color = color_black;

    } else {

      //Diffuse
      float diff = abs(dot(lightRay.getDirection(), intersection_data.normal));
      float falloff = light->getIntensity(length(light->getPosition()-intersection_data.interPoint));

      //Specular
      Ray refl = ray->reflection(lightRay, intersection_data.normal, intersection_data.interPoint);
      vec3 v = normalize(ray->getPosition()-intersection_data.interPoint);

      float spec;
      if(dot(intersection_data.normal,lightRay.getDirection()) < 0) {
        spec = 0.0f;
      } else {
        //spec = glm::max(0.0f,glm::pow( dot(refl.getDirection(),ray->getPosition()-intersection_data.interPoint),0.3f));
        spec = glm::max(0.0f,glm::pow( dot(normalize(refl.getDirection()),v),1.0f));
      }

      vec3 white = vec3(1,1,1);

      color  = falloff * (1.0f * spec * white + diff * texture_color);

      //color  = (texture_color);

      //color = falloff * diff * intersection_data.triangle->getMaterial()->getColor();
      // diff * intersection_data.triangle->getMaterial()->getColor()


      //cout << dot(lightRay.getDirection(), intersection_data.normal) << endl;

    }
    return vec4(color, material->getTransparency());
  }
}

}
