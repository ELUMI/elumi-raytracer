/*
 * PhotonMapper.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "PhotonMapper.h"
#include "../PhotonMapImpl/HashPM.h"
#include "../PhotonMapImpl/GridPM.h"
#include "../PhotonMapImpl/ArrayPM.h"
#include "../utilities/Random.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace raytracer {

PhotonMapper::PhotonMapper(Scene* scene)
: StandardTracer(scene) {
  switch(settings->photonmap) {
  case 0:
  case 2:
    photonmap = new ArrayPM(settings->photonmap_size);
    cout << "Using array photonmap\n";
    break;
  case 1:
  default:
    photonmap = new HashPM(settings->gather_radius, settings->photonmap_size);
    cout << "Using hash photonmap\n";
    break;
  }
  photon_processer = 0;
  colors = 0;
}

PhotonMapper::~PhotonMapper() {
  delete photonmap;
  if(photon_processer)
    delete photon_processer;
  if(colors)
    delete [] colors;
}

void PhotonMapper::initTracing(){
  StandardTracer::initTracing();
  if(!(settings->use_first_bounce && settings->photonmap==0)) {
    getPhotons();
  }
  photonmap->balance();
}

void PhotonMapper::first_bounce() {
  StandardTracer::first_bounce();
  if(settings->photonmap != 0)
    return;

  int width = settings->width;
  int height = settings->width;
  int size = width * height;

  if(!photon_processer) {
    getPhotons();

    photon_processer = new PhotonProcesser(width, height);

    assert(photonmap->getNumberOfBuckets() == 1);
    photon_processer->readPhotons(photonmap->getBucket(0));
  }

  mat4 view_matrix = scene->getCamera().getViewMatrix();
  GLuint normal_tex = first_pass->getNormalTex();
  GLuint depth_tex  = first_pass->getDepthTex();

  photon_processer->render(scene, width, height, normal_tex, depth_tex, settings->gather_radius, settings->photonmap_scaling);

  if(!colors) {
    colors = new vec3[size];
  }
  photon_processer->readColors(width, height, colors);

}

void PhotonMapper::storeInMap(Photon p){
  photonmap_mutex.lock();
  photonmap->addPhoton(p);
  photonmap_mutex.unlock();
}

bool PhotonMapper::bounce(Photon& p, int thread_id, bool store) {
  Ray ray(p.position, -p.direction);
  //cout << p.direction.x << " " << p.direction.y << " " << p.direction.z << "\n";

  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  if(idata.missed()){
    return false;
  }
  p.position = idata.interPoint;
  p.normal = idata.normal;

  Photon bp = p;

  Material* mat = scene->getMaterialVector()[idata.material];

  float absorbtion = 1.0f/10.0f; //fixed probability
  float reflection =    (1-absorbtion)                            * mat->getReflection();    //first priority
  float transmittance = (1-(absorbtion+reflection))               * (1 - mat->getOpacity()); //second priority
  float diffuse =       (1-(absorbtion+reflection+transmittance)) * 1;                       //third priority

  assert(reflection <= 1 && transmittance <= 1 && absorbtion <= 1 && diffuse <= 1);
  assert(reflection >= 0 && transmittance >= 0 && absorbtion >= 0 && diffuse >= 0);
  assert(reflection+transmittance+diffuse+absorbtion==1.0f);

  //cout << (1-mat->getOpacity()) << "\t";
  //cout << transmittance << "\t" << reflection << "\t" << diffuse << "\n";

  vec3 outgoing;
  const float refraction_sign = glm::sign(glm::dot(p.normal, p.direction));

  //russian roulette
  float rand = gen_random_float(thread_id);
  if (rand < transmittance || refraction_sign<0) {
    const vec3 refr_normal = p.normal * refraction_sign;
    float eta = mat->getIndexOfRefraction();
    if (refraction_sign < 0.0f)
      eta = 1 / eta;
    outgoing = glm::refract(-p.direction, refr_normal, eta);
    if(outgoing == vec3(0,0,0)) {
      return false;
    }
    //p.power *= vec3(0,1,1);
    //return false;
  } else if (rand < transmittance+reflection) {
    outgoing = glm::reflect(-p.direction, p.normal);
    p.power *= mat->getSpecular();
    //p.power *= vec3(1,1,0);
    //return false;
  } else if(rand < transmittance+reflection+diffuse) {  //diffuse interreflection
    if(store) {
      storeInMap(bp);
    }

    outgoing = gen_random_hemisphere(p.normal, thread_id);
    p.power *= 0.1f;
    p.power *= mat->getDiffuse();
    //return false;
  } else { //absorbtion
    if(store) {
      storeInMap(p);
    }

    return false;
  }

  p.direction = -outgoing;

  if((p.power.r + p.power.g + p.power.b)/3 < settings->recursion_attenuation_threshold)
    return false;

  return true;
}

void PhotonMapper::tracePhoton(Photon p, int thread_id)
{
  if(!bounce(p, thread_id, false))
    return;
  for(size_t k = 0;k < settings->max_recursion_depth;++k){
    if(abort)
      break;

    if(!bounce(p, thread_id))
      break;
  }
}

void PhotonMapper::tracePhotons(Photon* photons, size_t m, int thread_id) {
  //cout << "Thread:" << thread_id << " doing:\t" << m << "photons\n";
  for(int i=0;i<m;++i){
    tracePhoton(photons[i], thread_id);
  }
}


void PhotonMapper::getPhotons() {
  size_t n = settings->photons; //NUMBER_OF_PHOTONS;
  Photon *photons = new Photon[n];
  size_t pp = 0;

  float totalpower = 0;
  for(size_t i = 0;i < lights->size();++i){
    totalpower += lights->at(i)->getPower();
  }
  for(size_t i = 0;i < lights->size();++i){
    ILight *light = lights->at(i);
    float npe = n * light->getPower() / totalpower;
    size_t m = size_t(npe); //photons per light

    int thread_id=0;

    Ray *rays = new Ray[m];
    light->getRays(rays, m, thread_id);
    vec3 power = (1 / (light->getPower() / totalpower)) * light->getColor() * light->getIntensity();
    for(size_t j = 0;j < m;++j){
      if(abort)
        return;

      Ray ray = rays[j];
      Photon p;
      p.power = power;
      p.direction = -ray.getDirection();
      p.position = ray.getPosition();

      photons[pp++] = p;
    }

    delete [] rays;
  }

  if(abort)
    return;

  // Launch threads
  int nr_threads = settings->threads;
  if (nr_threads == 0)
    nr_threads = boost::thread::hardware_concurrency();
  boost::thread threads[nr_threads - 1];
  int m = n/nr_threads;
  for (int i = 0; i < nr_threads - 1; ++i) {
    threads[i] = boost::thread(
        boost::bind(&PhotonMapper::tracePhotons, this, photons+m*i, m, i));
  }
  tracePhotons(photons+m*(nr_threads-1), pp-m*(nr_threads-1), nr_threads-1);  //spawn one less thread by using this thread

  // Wait for threads to complete
  for (int i = 0; i < nr_threads - 1; ++i) {
    threads[i].join();
  }
  delete [] photons;
}

vector<Photon*> PhotonMapper::gather(float& r, vec3 point){
  r = settings->gather_radius;
  return photonmap->gatherFromR(point, r);
}

float PhotonMapper::filterKernel(vec3 offset, vec3 normal, float r) {
  //return 1/(M_PI*r*r); //simple filter kernel

  //advanced filter kernel (ISPM paper)
  const float sz = 0.1;
  float dist = length(offset);
  float t = (dist / r) * (1 - dot(offset / dist, normal) * (r + sz*r) / sz);
  float sigma = 0.4; //t=1,k<0.1 => sigma<0.45
  return exp(-t*t/(2*sigma*sigma));
}

vec3 PhotonMapper::getLuminance(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata) {
  Material *material = scene->getMaterialVector()[idata.material];

  vec3 l = vec3(0);

  //size_t g; //number of photons
  float r;  //gather radius

  vector<Photon*> photons = gather(r, idata.interPoint);
  if(photons.size() == 0)
    return vec3(0);
  for(size_t i=0; i<photons.size(); ++i){
    const Photon* p = photons[i];

    vec3 b = brdf(-p->direction, incoming_ray.getDirection(), idata.normal, material);
    float k = filterKernel(idata.interPoint - p->position, idata.normal, r);
    float a = glm::max(0.0f, glm::dot(p->direction, idata.normal));
    //cout << p.power.r << " " << p.power.g << " " << p.power.b << "\n";
    //cout << b << " " << a << " " << k << "\n";
    l += b * p->power * a * k;
    //l += a;
  }
  l /= photonmap->getTotalPhotons();
  l *= settings->photonmap_scaling; //magic scaling

  return l;
}

vec3 PhotonMapper::getAmbient(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, int thread_id, unsigned short depth) {
  const size_t final_gather_samples = settings->final_gather_samples;
  vec3 l = vec3(0);
  if(final_gather_samples != 0){
    //final gather
    l = vec3(0);
    for(size_t i = 0;i < final_gather_samples;++i){
      Ray ray = Ray(idata.interPoint, gen_random_hemisphere(idata.normal, thread_id));
      IAccDataStruct::IntersectionData idata2 = datastruct->findClosestIntersection(ray);
      if(!idata2.missed()){
        l += getLuminance(ray, idata2);
      }
    }

    l /= final_gather_samples;
  } else {
    if(settings->use_first_bounce && colors && depth == 0) {
      //l = getLuminance(incoming_ray, idata);
      return colors[position[thread_id]];
    } else {
      l = getLuminance(incoming_ray, idata);
    }
  }
  return l;
}

vec4 PhotonMapper::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata,
    float attenuation, unsigned short depth, int thread_id) {
  if(idata.missed()){
    // No intersection
    return settings->background_color;
  }

  Material* mat = scene->getMaterialVector()[idata.material];
  vec3 l = getAmbient(incoming_ray, idata, thread_id, depth);
  vec3 color = mat->getDiffuse();
  return vec4(l,1);
  return vec4(l*color+mat->getEmissive(),1);
}

}
