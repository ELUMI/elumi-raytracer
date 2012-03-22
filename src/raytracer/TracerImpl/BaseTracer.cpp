/*
 * BaseTracer.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: irri
 */

#include "BaseTracer.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective
namespace raytracer {


BaseTracer::BaseTracer(Scene* scene, Settings* settings) {
  this->scene = scene;
  this->settings = settings;

  datastruct = scene->getAccDataStruct();

  first_pass = 0;
  first_intersections = 0;

  posbuff = new vec3[settings->width * settings->height];

  abort = false;
}

BaseTracer::~BaseTracer() {
  stopTracing();
  delete[] rays;
  delete[] posbuff;
}

void BaseTracer::first_bounce() {
  if (!settings->use_first_bounce) {
    return;
  }
  if (settings->opengl_version < 3) {
    settings->use_first_bounce = false;
    return;
  }

  int width = settings->width;
  int height = settings->width;
  int size = width * height;

  if (first_pass) {
    delete first_pass; //there may be a unneccesary delete here, which may be very slow
    delete[] first_intersections;
  }
  first_pass = new DeferredProcesser(settings->width, settings->height); //TODO: use settings, and check useopengl
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

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int i = y * width + x;
      vec4 pos = vp2m * vec4(x + 0.5, y + 0.5, depths[i], 1);

      //see comment in deferred.frag
      unsigned int material = ceil(normals[i].w - 0.5); //alpha channel is noisy, but this works!
      assert(material == IAccDataStruct::IntersectionData::NOT_FOUND || material < scene->getMaterialVector().size());

      first_intersections[i] = IAccDataStruct::IntersectionData(material,
          vec3(pos) / pos.w, vec3(normals[i]), texcoords[i]);
    }
  }

  delete[] normals;
  delete[] texcoords;
  delete[] depths;
}

void BaseTracer::tracePixel(size_t i, IAccDataStruct::IntersectionData intersection_data) {
  posbuff[i] = intersection_data.interPoint; //put this before so trace can overwrite it
  vec4 c = trace(rays[i], intersection_data);
  buffer[i * 4] = c.r;
  buffer[i * 4 + 1] = c.g;
  buffer[i * 4 + 2] = c.b;
  buffer[i * 4 + 3] = c.a;
}


void BaseTracer::initTracing()
{
  lights = scene->getLightVector();
  pixelsLeft = settings->width * settings->height;
  abort = false;
  cout << "camera.set(vec3(" << scene->getCamera().getPosition().x
       << "," << scene->getCamera().getPosition().y
       << "," << scene->getCamera().getPosition().z
       << "), vec3(" << scene->getCamera().getDirection().x
       << "," << scene->getCamera().getDirection().y
       << "," << scene->getCamera().getDirection().z
       << "), vec3(" << scene->getCamera().getUpVector().x
       << "," << scene->getCamera().getUpVector().y
       << "," << scene->getCamera().getUpVector().z
       << "), 0.7845f, settings.width/settings.height);\n";
}

void BaseTracer::traceImage(float *color_buffer)
{
  buffer = color_buffer;
  initTracing();
  int number_of_rays = spawnRays();

  if(settings->use_first_bounce){
    // For every pixel
    #pragma omp parallel for
    for(int i = 0;i < number_of_rays;++i){
      //i must be signed according to openmp
      if(!abort){
        tracePixel(i, first_intersections[i]);
        #pragma omp atomic
        --pixelsLeft;
      }
    }

  }
  else{
    // For every pixel
    #pragma omp parallel for
    for(int i = 0;i < number_of_rays;++i){
      //i must be signed according to openmp
      if(!abort){
        IAccDataStruct::IntersectionData intersection_data = scene->getAccDataStruct()->findClosestIntersection(rays[i]);
        tracePixel(i, intersection_data);
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
  int width = settings->width;
  int height = settings->height;

  // Number of rays to spawn from camera
  int number_of_rays = width * height;
  // Initiate ray array
  rays = new Ray[number_of_rays]; //TODO om man renderar många gånger i samma körning???

  Camera camera = scene->getCamera();
  vec3 camera_position = camera.getPosition();
  mat4 trans = camera.getViewportToModelMatrix(width - 1, height - 1);

  //We step over all "pixels" from the cameras viewpoint
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      vec4 aray = vec4(x, y, -1, 1);
      //vec3 r = unProject(vec3(aray), trans, mat4(), vec4(0, 0, width, height));
      aray = trans * aray;
      vec3 r = vec3(aray / aray.w);
      rays[y * width + x] = Ray::generateRay(camera_position, r);
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
  float light = 0;
  for(size_t i = 0; i < lights->size(); ++i){
    //if(!lights->at(i)->isBlocked(datastruct, idata.interPoint)){
    if (lights->at(i)->calcLight(datastruct, idata.interPoint) > 0.0f) {
      light++;
    }
  }
  light /= lights->size();
  vec3 color = scene->getMaterialVector()[idata.material]->getDiffuse();
  return vec4(color * light, 1);
}

}
