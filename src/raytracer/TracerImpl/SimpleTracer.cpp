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
  first_pass = new DeferredProcesser(100,100); //TODO: use settings, and check useopengl
  first_intersections = new IAccDataStruct::IntersectionData[100*100];
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

void SimpleTracer::first_bounce() {
  if(first_pass){
    mat4 vp2m = scene->getCamera().getViewportToModelMatrix(100,100);
    mat4 view_matrix = scene->getCamera().getViewMatrix();
    first_pass->render(scene, view_matrix);

    vec3* normals = new vec3[100*100];
    vec3* texcoords = new vec3[100*100];
    float* depths = new float[100*100];

    first_pass->readNormals(100,100, normals);
    first_pass->readTexCoords(100,100, texcoords);
    first_pass->readDepths(100,100, depths);

    for(int y=0; y<100; y++){
      for(int x=0; x<100; x++){
        int i = y*100+x;
        vec4 pos = vp2m * vec4(x,y,depths[i],1);

        //see comment in deffered.frag
        unsigned int material = int(256.0f * texcoords[i].z);
        if(material == 0){
          material = IAccDataStruct::IntersectionData::NOT_FOUND;
        } else {
          --material;
        }
        assert(material == IAccDataStruct::IntersectionData::NOT_FOUND || material < scene->getMaterialVector().size());

        first_intersections[i] = IAccDataStruct::IntersectionData(
            material, vec3(normalize(pos)), normals[i]);
      }
    }

    delete [] normals;
    delete [] texcoords;
    delete [] depths;
  }
}

void SimpleTracer::trace(Ray* rays, int length, uint8_t* buffer) {
  abort = false;
  SimpleTracer::buffer = buffer;
  pixelsLeft = length;

  if(first_intersections) {
    #pragma omp parallel for
    for (size_t i=0; i<length; ++i) {
      //#pragma omp task
      #pragma omp flush (abort)
      if(!abort)
      {
        vec4 c = traceHelper(&rays[i], first_intersections[i]);
        buffer[i*4] = 255*glm::min(1.0f, c.r);
        buffer[i*4 +1] = 255*glm::min(1.0f, c.g);
        buffer[i*4 +2] = 255*glm::min(1.0f, c.b);
        buffer[i*4 +3] = 255*glm::min(1.0f, c.a);

        #pragma omp atomic
        --pixelsLeft;
      }
    }
  } else {
    #pragma omp parallel for
    for (size_t i=0; i<length; ++i) {
      //#pragma omp task
      #pragma omp flush (abort)
      if(!abort)
      {
        IAccDataStruct::IntersectionData intersection_data =
            scene->getAccDataStruct()->findClosestIntersection(rays[i]);
        vec4 c = traceHelper(&rays[i], intersection_data);
        buffer[i*4] = 255*glm::min(1.0f, c.r);
        buffer[i*4 +1] = 255*glm::min(1.0f, c.g);
        buffer[i*4 +2] = 255*glm::min(1.0f, c.b);
        buffer[i*4 +3] = 255*glm::min(1.0f, c.a);

        #pragma omp atomic
        --pixelsLeft;
      }
    }
  }
}

vec4 SimpleTracer::traceHelper(Ray* ray, IAccDataStruct::IntersectionData intersection_data, int levels) {
  vec3 color_black = vec3(0.0f,0.0f,0.0f);

  if (intersection_data.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    return background_color;

  } else {
    Material* material = scene->getMaterialVector()[intersection_data.material];

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
      Ray invLightRay = Ray(intersection_data.interPoint, -lightRay.getDirection());
      Ray refl = ray->reflection(invLightRay, intersection_data.normal, intersection_data.interPoint);
      vec3 v = normalize(ray->getPosition()-intersection_data.interPoint);

      float spec;
      if(dot(intersection_data.normal,-lightRay.getDirection()) < 0) {
        spec = 0.0f;
      } else {
        //spec = glm::max(0.0f,glm::pow( dot(refl.getDirection(),ray->getPosition()-intersection_data.interPoint),0.3f));
        spec = glm::max(0.0f,glm::pow( dot(normalize(refl.getDirection()),v),1.0f));
      }

      vec3 white = vec3(1,1,1);

      color  = falloff * (1.0f * spec * white + diff * material->getDiffuse());
      //color = falloff * diff * intersection_data.triangle->getMaterial()->getColor();
      // diff * intersection_data.triangle->getMaterial()->getColor()


      //cout << dot(lightRay.getDirection(), intersection_data.normal) << endl;

    }
    return vec4(color, material->getTransparency());
  }
}

}
