/*
 * SimpleTracer.cpp
 *
 *  Created on: Jan 26, 2012
 *      Author: david
 */

#include <iostream>

#include "SimpleTracer.h"
#include <glm/glm.hpp>

using namespace glm;

namespace raytracer {

//ITracer::~ITracer() {}

SimpleTracer::SimpleTracer(Scene* scene, Settings* settings) : BaseTracer(scene, settings) {
}

SimpleTracer::~SimpleTracer() {
}


void SimpleTracer::traceImage(float* color_buffer) {
  buffer = color_buffer;
  pixelsLeft = settings->width*settings->height;
  abort = false;
  createRays();
}

void SimpleTracer::createRays()  {

  int rays_length = settings->width*settings->height;

  //Initiate ray array
  Ray* rays = new Ray[rays_length];

  Camera camera = scene->getCamera();
  mat4 view = camera.getViewMatrix();
  view = inverse(view);

  //We step over all "pixels" from the cameras viewpoint
  for(int h = 0; h < settings->height; h++) {
    for(int w = 0; w < settings->width; w++) {
      vec4 dir = vec4(2.0f*float(w)/(settings->width -1)-1.0f,
          2.0f*float(h)/(settings->height-1)-1.0f, 1.0f, 1.0f);
      dir = view*dir;

      rays[w+h*settings->width] = Ray(camera.getPosition(),vec3(dir));

    }
  }

  if(settings->use_first_bounce) {
    #pragma omp parallel for
    for (size_t i=0; i<rays_length; ++i) {
      //#pragma omp task
      #pragma omp flush (abort)
      if(!abort)
      {
        vec4 c = traceHelper(&rays[i], first_intersections[i]);
        buffer[i*4] = glm::min(1.0f, c.r);
        buffer[i*4 +1] = glm::min(1.0f, c.g);
        buffer[i*4 +2] = glm::min(1.0f, c.b);
        buffer[i*4 +3] = glm::min(1.0f, c.a);

        #pragma omp atomic
        --pixelsLeft;
      }
    }
  } else {
    #pragma omp parallel for
    for (size_t i=0; i<rays_length; ++i) {
      //#pragma omp task
      #pragma omp flush (abort)
      if(!abort)
      {
        IAccDataStruct::IntersectionData intersection_data =
            scene->getAccDataStruct()->findClosestIntersection(rays[i]);
        vec4 c = traceHelper(&rays[i], intersection_data);
        buffer[i*4] = glm::min(1.0f, c.r);
        buffer[i*4 +1] = glm::min(1.0f, c.g);
        buffer[i*4 +2] = glm::min(1.0f, c.b);
        buffer[i*4 +3] = glm::min(1.0f, c.a);

        #pragma omp atomic
        --pixelsLeft;
      }
    }
  }

  //Free memory
  delete [] rays;
}

vec4 SimpleTracer::traceHelper(Ray* ray, IAccDataStruct::IntersectionData intersection_data) {
  vec3 color_black = vec3(0.0f,0.0f,0.0f);

  if (intersection_data.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    return settings->background_color;
  } else {
    Material* material = scene->getMaterialVector()[intersection_data.material];

    vec3 texture_color = vec3(0,0,0);

    vec3 bump_normal = vec3(0,0,0);

    /*if(material->getTexture() != -1) {

      Texture* texture = scene->getTextureAt(material->getTexture());

      vec3 p = intersection_data.interPoint;
      vec2 texCoords = vec2(0,0);

      float x;
      float y;

      //Planar mapping
      if(intersection_data.uvcoords.size() ==  0) {
        x = p.x;
        y = p.y;

        if(x > y) {
          x = p.y;
          y = p.x;
        }
        if(y > p.z) {
          y = p.z;
        }

        texCoords.x = x;
        texCoords.y = y;

      } else { //We have texture coordinatesalf

        texCoords.x = intersection_data.texcoord.x * p.x;
        texCoords.y = intersection_data.texcoord.y * p.y;

        /*texCoords.x = alfa.x * coords[0]->x + alfa.y * coords[1]->x + alfa.z * coords[2]->x;
        texCoords.y = alfa.x * coords[0]->y + alfa.y * coords[1]->y + alfa.z * coords[2]->y;

      }

      texCoords.x = (int)(texCoords.x*(texture->getWidth()/2))%texture->getWidth();
      texCoords.y = (int)(texCoords.y*(texture->getHeight()/2))%texture->getHeight();

      int x_coord = (int)texCoords.x;
      int y_coord = (int)texCoords.y;

      texture_color = texture->getColorAt(x_coord,y_coord)/255.0f;

    }*/

    vec3 color;
    //Light
    ILight* light = scene->getLightVector().front();
    vec3 dir = normalize(intersection_data.interPoint - light->getPosition());
    Ray lightRay = Ray(light->getPosition(), dir);
    IAccDataStruct::IntersectionData intersection_data_light = scene->getAccDataStruct()->findClosestIntersection(lightRay);

    if (dot(dir,(intersection_data.interPoint - intersection_data_light.interPoint)) > 0.001) {
      color = color_black;

    } else {

      vec3 normal = glm::normalize(intersection_data.normal);

      //Diffuse
      float diff = abs(dot(lightRay.getDirection(), normal));
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

      color  = falloff * (1.0f * spec * white + diff * texture_color);

      //color  = falloff * (diff * texture_color);

      //color  = (texture_color);

      //color = falloff * diff * intersection_data.triangle->getMaterial()->getColor();
      // diff * intersection_data.triangle->getMaterial()->getColor()


      //cout << dot(lightRay.getDirection(), intersection_data.normal) << endl;

    }
    return vec4(color, material->getTransparency());
  }
}

}
