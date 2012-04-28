/*
 * BaseTracer.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: irri
 */

#include <iostream>

#include "BaseTracer.h"
#include <omp.h>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>

#include "../GLData/DeferredTexProcesser.h"

#include "../utilities/Random.h"
#include "../RenderPatternImpl/LinePattern.h"
#include "../RenderPatternImpl/HilbertCurve.h"

#include "../SuperSamplePatternImpl/GridPattern.h"
#include "../SuperSamplePatternImpl/RandomPattern.h"
#include "../SuperSamplePatternImpl/JitterPattern.h"

namespace raytracer {

BaseTracer::BaseTracer(Scene* scene) {
  this->scene = scene;
  datastruct = scene->getAccDataStruct();

  first_pass = 0;
  first_intersections = 0;
  settings = scene->getSettings();

  posbuff = new vec3[settings->width * settings->height];

  abort = false;
  position = 0;
}

BaseTracer::~BaseTracer() {
  stopTracing();
  delete [] posbuff;

  if (first_pass) {
    delete first_pass;
    delete first_intersections;
  }
  if(position)
    delete position;
}

void BaseTracer::runWithGL() {
  if (!settings->use_first_bounce || settings->opengl_version<3) {
    return;
  }
  first_bounce();
}

void BaseTracer::first_bounce() {
  int width = settings->width;
  int height = settings->width;
  int size = width * height;

  if (!first_pass) {
    first_pass = new DeferredTexProcesser(settings->width, settings->height);
    first_intersections = new IAccDataStruct::IntersectionData[size];
  }

  mat4 vp2m = scene->getCamera().getViewportToModelMatrix(width, height);
  mat4 view_matrix = scene->getCamera().getViewMatrix();
  first_pass->render(scene, view_matrix, width, height);

  vec3* normals = new vec3[size];
  vec3* texcoords = new vec3[size];
  float* depths = new float[size];

  first_pass->readNormals(width, height, normals);
  first_pass->readTexCoords(width, height, texcoords);
  first_pass->readDepths(width, height, depths);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int i = y * width + x;
      vec4 pos = vp2m * vec4(x + 0.5, y + 0.5, depths[i], 1);

      //see comment in deferred.frag
      unsigned int material = ceil(texcoords[i].z - 0.5); //alpha channel is noisy, but this works!
      //cout << texcoords[i].z << "\t" << material << "\n";
      assert(material == IAccDataStruct::IntersectionData::NOT_FOUND_INTERNAL || material < scene->getMaterialVector().size());

      first_intersections[i] = IAccDataStruct::IntersectionData(NULL, material,
          vec3(pos) / pos.w, normals[i], vec2(texcoords[i]),vec3(),vec3());

    }
  }

  delete [] normals;
  delete [] texcoords;
  delete [] depths;
}

void BaseTracer::initTracing()
{
  if (settings->pattern == 1)
    pattern = new HilbertCurve(settings->width, settings->height, settings->batches);
  else
    pattern = new LinePattern(settings->width, settings->height);

  nr_batches = pattern->getNumberBatches();
  next_batch = 0;

  lights = scene->getLightVector();
  abort = false;
  cout << "<Position x=\"" << scene->getCamera().getPosition().x
      << "\" y=\"" << scene->getCamera().getPosition().y
      << "\" z=\"" << scene->getCamera().getPosition().z
      << "\"/>\n<Direction x=\"" << scene->getCamera().getDirection().x
      << "\" y=\"" << scene->getCamera().getDirection().y
      << "\" z=\"" << scene->getCamera().getDirection().z
      << "\"/>\n<Normal x=\"" << scene->getCamera().getUpVector().x
      << "\" y=\"" << scene->getCamera().getUpVector().y
      << "\" z=\"" << scene->getCamera().getUpVector().z << "\"/>\n";
}

void BaseTracer::initThreads(int nr_threads) {
  // Init shadow caches
  for (size_t i = 0; i < lights->size(); ++i) {
    //lights->at(i)->initCaches(nr_threads);
  }

  if(position)
    delete position;
  position = new size_t[nr_threads];
}

void BaseTracer::traceImage(float* color_buffer) {
  buffer = color_buffer;
  // Launch threads
  int nr_threads = settings->threads;
  if (nr_threads == 0)
    nr_threads = boost::thread::hardware_concurrency();

  // Init shadow caches
  initThreads(nr_threads);
  boost::thread threads[nr_threads - 1];
  for (int i = 0; i < nr_threads - 1; ++i) {
    threads[i] = boost::thread(
        boost::bind(&BaseTracer::traceImageThread, this, i));
  }
  traceImageThread(nr_threads-1); //spawn one less thread by using this thread
  // Wait for threads to complete
  for (int i = 0; i < nr_threads - 1; ++i) {
    threads[i].join();
  }
  delete pattern;
}

void BaseTracer::traceImageThread(int thread_id) {
  // Synchronize work
  int my_batch = 0;
  pattern_mutex.lock();
  my_batch = next_batch++;
  pattern_mutex.unlock();
  int width = settings->width;
  int height = settings->height;
  Camera camera = scene->getCamera();
  vec3 camera_position = camera.getPosition();
  mat4 trans = camera.getViewportToModelMatrix(width, height);
  ISuperSamplePattern* ss_pattern;
  switch (settings->super_sampler_pattern) {
  case 0:
  default:
    ss_pattern = new GridPattern(settings->samples);
    break;
  case 1:
    ss_pattern = new RandomPattern(settings->samples, thread_id);
    break;
  case 2:
    ss_pattern = new JitterPattern(settings->samples, thread_id);
    break;
  }
  while (my_batch < nr_batches) {
    //cout << "Thread: " << id << " on batch: " << my_batch << endl;
    int length;
    int* batch = pattern->getBatch(my_batch, &length);
    for (size_t b = 0; b < length; ++b) {
      if (abort) {
        return;
      }
      int i = batch[b];
      vec2 coord = vec2(i % width, i / width);
      const vec2* offsets = ss_pattern->getNewOffsets();
      vec4 c = vec4(0);
      for (int s = 0; s < ss_pattern->getSize(); ++s) {
        vec4 pos = trans
            * vec4(coord.x + offsets[s].x, coord.y + offsets[s].y, -1, 1);
        Ray ray = Ray::generateRay(camera_position, vec3(pos / pos.w));
        IAccDataStruct::IntersectionData intersection_data;
        if (settings->use_first_bounce) {
          intersection_data = first_intersections[i];
        } else {
          intersection_data =
              scene->getAccDataStruct()->findClosestIntersection(ray);
        }
        if (s == 0) {
          posbuff[i] = intersection_data.interPoint;
        }
        position[thread_id] = i;
        c += trace(ray, intersection_data, thread_id);
      }

      c /= ss_pattern->getSize();
      buffer[i * 4] = c.r;
      buffer[i * 4 + 1] = c.g;
      buffer[i * 4 + 2] = c.b;
      buffer[i * 4 + 3] = c.a;
    }

    pattern_mutex.lock();
    my_batch = next_batch++;
    pattern_mutex.unlock();
  }

  delete ss_pattern;
}

void BaseTracer::stopTracing() {
  abort = true;
}

float BaseTracer::getPixelsLeft() {
  return clamp((float) ((nr_batches - next_batch)) / (float) (nr_batches));
}

vec4 BaseTracer::trace(Ray ray, IAccDataStruct::IntersectionData idata, int thread_id) {
  vec4 color;

  if (idata.missed()) {
    // No intersection
    color = settings->background_color;
  } else {
    // Intersection!
    color = shade(ray, idata, thread_id);
  }

  return color;
}

vec4 BaseTracer::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata, int thread_id) {
  float light = 0;
  for(size_t i = 0; i < lights->size(); ++i){
    //if(!lights->at(i)->isBlocked(datastruct, idata.interPoint)){
    if (lights->at(i)->calcLight(datastruct, idata.interPoint, thread_id) > 0.0f) {
      light++;
    }
  }
  light /= lights->size();
  vec3 color = scene->getMaterialVector()[idata.material]->getDiffuse();
  return vec4(color * light, 1);
}

}
