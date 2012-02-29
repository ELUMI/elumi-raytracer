/*
 * BaseTracer.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: irri
 */

#include "BaseTracer.h"

namespace raytracer {

BaseTracer::BaseTracer(Scene* scene, Settings* settings) : lights(&(scene->getLightVector())) {
  this->scene = scene;
  this->settings = settings;

  datastruct = scene->getAccDataStruct();

  first_pass = 0;

  abort = false;
}

BaseTracer::~BaseTracer() {
  stopTracing();
  delete [] rays;
}

void BaseTracer::first_bounce() {
  if(!settings->use_first_bounce){
    return;
  }

  int width = settings->width;
  int height= settings->width;
  int size= width*height;

  if(first_pass) {
    delete first_pass; //there may be a unneccesary delete here, which may be very slow
    delete first_intersections;
  }
  first_pass = new DeferredProcesser(settings->width,settings->height); //TODO: use settings, and check useopengl
  first_intersections = new IAccDataStruct::IntersectionData[size];

  mat4 vp2m = scene->getCamera().getViewportToModelMatrix(width, height);
  mat4 view_matrix = scene->getCamera().getViewMatrix();
  first_pass->render(scene, view_matrix, width, height);

  vec3* normals = new vec3[size];
  vec3* texcoords = new vec3[size];
  float* depths = new float[size];

  first_pass->readNormals(width, height, normals);
  first_pass->readTexCoords(width, height, texcoords);
  first_pass->readDepths(width, height, depths);

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      int i = y*width+x;
      vec4 pos = vp2m * vec4(x,y,depths[i],1);

      //see comment in deferred.frag
      unsigned int material = int(256.0f * texcoords[i].z);
      if(material == 0){
        material = IAccDataStruct::IntersectionData::NOT_FOUND;
      } else {
        --material;
      }
      assert(material == IAccDataStruct::IntersectionData::NOT_FOUND || material < scene->getMaterialVector().size());

      first_intersections[i] = IAccDataStruct::IntersectionData(
          material, vec3(normalize(pos)),
          normals[i], vec2(texcoords[i].x, texcoords[i].y));
    }
  }

  delete [] normals;
  delete [] texcoords;
  delete [] depths;
}

void BaseTracer::traceImage(float* color_buffer) {
  buffer = color_buffer;
  pixelsLeft = settings->width*settings->height;
  abort = false;

  int number_of_rays = spawnRays();

  if(settings->use_first_bounce) {
    // For every pixel
    #pragma omp parallel for
    for (size_t i=0; i<number_of_rays; ++i) {
      //#pragma omp task
      #pragma omp flush (abort)
      if(!abort)
      {
        vec4 c = trace(rays[i], first_intersections[i]);
        buffer[i*4] = glm::min(1.0f, c.r);
        buffer[i*4 +1] = glm::min(1.0f, c.g);
        buffer[i*4 +2] = glm::min(1.0f, c.b);
        buffer[i*4 +3] = glm::min(1.0f, c.a);

        #pragma omp atomic
        --pixelsLeft;
      }
    }
  } else {
    // For every pixel
    #pragma omp parallel for
    for (size_t i=0; i<number_of_rays; ++i) {
      //#pragma omp task
      #pragma omp flush (abort)
      if(!abort)
      {
        IAccDataStruct::IntersectionData intersection_data =
            scene->getAccDataStruct()->findClosestIntersection(rays[i]);
        vec4 c = trace(rays[i], intersection_data);
        buffer[i*4] = glm::min(1.0f, c.r);
        buffer[i*4 +1] = glm::min(1.0f, c.g);
        buffer[i*4 +2] = glm::min(1.0f, c.b);
        buffer[i*4 +3] = glm::min(1.0f, c.a);

        #pragma omp atomic
        --pixelsLeft;
      }
    }
  }

}

void BaseTracer::stopTracing() {
  abort = true;
}

unsigned int BaseTracer::getPixelsLeft() {
  return pixelsLeft;
}

int BaseTracer::spawnRays() {
  // Local variables
  int width  = settings->width;
  int height = settings->height;

  // Number of rays to spawn from camera
  int number_of_rays = width*height;
  // Initiate ray array
  rays = new Ray[number_of_rays];

  Camera camera = scene->getCamera();
  vec3 camera_position = camera.getPosition();
  mat4 trans = camera.getViewportToModelMatrix(width, height);

  //We step over all "pixels" from the cameras viewpoint
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      vec4 apoint = vec4(0,0,0,1);
      vec4 aray = vec4(x,y,settings->test,1);
      apoint = trans * apoint;
      //aray = transpose(inverse(trans)) * aray;
      aray = trans * aray;
      vec3 p = vec3(apoint); ///apoint.w;
      vec3 r = normalize(vec3(aray));
      rays[y*width+x] = Ray(camera_position,r);
    }
  }

  return number_of_rays;
}

vec4 BaseTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata) {
  vec4 color;

  if (idata.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    // No intersection
    color = settings->background_color;
  } else {
    // Intersection!

    color = shade(ray, idata);

  }

  return color;
}

vec4 BaseTracer::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata) {
  return vec4(idata.normal,1);
}

}
