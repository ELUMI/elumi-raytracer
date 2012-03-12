/*
 * BaseTracer.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: irri
 */

#include "BaseTracer.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective

namespace raytracer {

BaseTracer::BaseTracer(Scene* scene, Settings* settings) : lights((scene->getLightVector())) {
  this->scene = scene;
  this->settings = settings;

  datastruct = scene->getAccDataStruct();

  first_pass = 0;
  first_intersections = 0;

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
    delete [] first_intersections;
  }
  first_pass = new DeferredProcesser(settings->width,settings->height); //TODO: use settings, and check useopengl
  first_intersections = new IAccDataStruct::IntersectionData[size];

  mat4 vp2m = scene->getCamera().getViewportToModelMatrix(width, height);
  mat4 view_matrix = scene->getCamera().getViewMatrix();
  first_pass->render(scene, view_matrix, width, height);

  vec4* normals = new vec4[size];
  vec2* texcoords = new vec2[size];
  float* depths = new float[size];

  first_pass->readNormals(width, height, normals);
  first_pass->readTexCoords(width, height, texcoords);
  first_pass->readDepths(width, height, depths);

  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      int i = y*width+x;
      vec4 pos = vp2m * vec4(x,y,depths[i],1);

      //see comment in deferred.frag
      unsigned int material = ceil(normals[i].w-0.5); //alpha channel is noise, but this works!
      assert(material == IAccDataStruct::IntersectionData::NOT_FOUND || material < scene->getMaterialVector().size());

      first_intersections[i] = IAccDataStruct::IntersectionData(
          material, vec3(pos)/pos.w,
          vec3(normals[i]), texcoords[i]);
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

  cout << "Camera: Pos=" << scene->getCamera().getPosition().x << ","
                         << scene->getCamera().getPosition().y << ","
                         << scene->getCamera().getPosition().z << ","
       << "; Dir=" << scene->getCamera().getDirection().x << ","
                   << scene->getCamera().getDirection().y << ","
                   << scene->getCamera().getDirection().z << ","
       << "; Up=" << scene->getCamera().getUpVector().x << ","
                  << scene->getCamera().getUpVector().y << ","
                  << scene->getCamera().getUpVector().z << ","
       << "\n";

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
  mat4 trans = camera.getViewportToModelMatrix(width-1, height-1);

  //We step over all "pixels" from the cameras viewpoint
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      vec4 aray = vec4(x,y,-1,1);
      //vec3 r = unProject(vec3(aray), trans, mat4(), vec4(0, 0, width, height));
      aray = trans * aray;
      vec3 r = vec3(aray / aray.w);
      rays[y*width+x] = Ray::generateRay(camera_position, r);
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

  switch(settings->debug_mode) {
  case 1:
    return vec4(idata.material/5.0f,idata.material/10.0f,0,1);
  case 2:
    return vec4(idata.normal,1);
  case 3:
    return vec4(scene->getMaterialVector()[idata.material]->getDiffuse(),1);
  case 4:
    return vec4(idata.normal,1);
  case 5:
    return vec4(idata.texcoord,0,1);
  case 6: {
    IAccDataStruct::IntersectionData idata2 = scene->getAccDataStruct()->findClosestIntersection(incoming_ray);
    return vec4(normalize(idata.normal) - normalize(idata2.normal),1);
  }
  case 7: {
    IAccDataStruct::IntersectionData idata2 = scene->getAccDataStruct()->findClosestIntersection(incoming_ray);
    return vec4(abs(normalize(idata.normal) - normalize(idata2.normal))*100.0f,1);
  }
  case 8: {
    IAccDataStruct::IntersectionData idata2 = scene->getAccDataStruct()->findClosestIntersection(incoming_ray);
    return vec4(abs(idata.interPoint-idata2.interPoint)*10.0f,1);
  }

  }

  ILight* light = scene->getLightVector().front();
  vec3 dir = normalize(idata.interPoint - light->getPosition());
  Ray lightRay = Ray(light->getPosition(), dir);
  IAccDataStruct::IntersectionData idatal = scene->getAccDataStruct()->findClosestIntersection(lightRay);
  assert(idatal.material != IAccDataStruct::IntersectionData::NOT_FOUND);

  if (dot(dir,(idata.interPoint - idatal.interPoint)) > 0.001+settings->test) {
    return vec4();
  } else {
    Material* material = scene->getMaterialVector()[idata.material];
    return vec4(material->getDiffuse(),1);
  }

}

}
