/*
 * PhotonMapper.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "PhotonMapper.h"
#include "../PhotonMapImpl/HashPM.h"
#include "../utilities/Random.h"

namespace raytracer {

PhotonMapper::PhotonMapper(Scene* scene)
: StandardTracer(scene) {
  radius = settings->gather_radius;
  photonmap = new HashPM(radius, settings->photonmap_size);
}

PhotonMapper::~PhotonMapper() {
  // TODO Auto-generated destructor stub
  delete photonmap;
}

void PhotonMapper::initTracing(){
  StandardTracer::initTracing();
  getPhotons();
  photonmap->balance();
}

void PhotonMapper::storeInMap(Photon p){
  photonmap->addPhoton(p);
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

  if(store) {
    storeInMap(p);
  }

  Material* mat = scene->getMaterialVector()[idata.material];

  float reflection = mat->getReflection()/3;
  float refraction = mat->getReflection()/3;
  float absorbtion = 1/3;
  float diffuse    = 1 - reflection - refraction - absorbtion;
  vec3 outgoing;

  assert(reflection <= 1 && refraction <= 1 && absorbtion <= 1 && diffuse <= 1);
  assert(reflection+refraction+diffuse+absorbtion==1.0f);

  //russian roulette
  float rand = gen_random_float(thread_id);
  if (rand < reflection) {
    outgoing = glm::reflect(p.direction, p.normal);
    p.power *= mat->getSpecular();
  } else if (rand < reflection+refraction) {
    outgoing = glm::refract(p.direction, p.normal, mat->getIndexOfRefraction());
    p.power *= mat->getDiffuse();
  } else if(rand < reflection+refraction+diffuse) {  //diffuse interreflection
    outgoing = gen_random_hemisphere(p.normal, thread_id);
    p.power *= mat->getDiffuse();
  } else { //absorbtion
    return false;
  }

  p.direction = -outgoing;

  if((p.power.r + p.power.g + p.power.b)/3 < settings->recursion_attenuation_threshold)
    return false;

  return true;
}

void PhotonMapper::tracePhoton(Photon p, int thread_id)
{
  for(size_t k = 0;k < settings->max_recursion_depth;++k){
    if(abort)
      break;

    if(!bounce(p, thread_id))
      break;
  }
}

void PhotonMapper::getPhotons()
{
  size_t n = settings->photons; //NUMBER_OF_PHOTONS;
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

      tracePhoton(p, thread_id);
    }

    delete [] rays;
  }
}

vector<Photon*> PhotonMapper::gather(float& r, vec3 point){
  r = radius;
  return photonmap->gatherFromR(point, r);
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
    Photon* p = photons[i];

    vec3 b = brdf(-p->direction, incoming_ray.getDirection(), idata.normal, material);

    float k;
    //k = 1/(M_PI*r*r); //simple filter kernel

    { //advanced filter kernel (ISPM paper)
      float dist = length(idata.interPoint-p->position);
      float rz = 0.1 * r;
      float t = (dist/r)*(1-dot((idata.interPoint-p->position) / dist, idata.normal)*(r+rz)/rz);
      float scale = 0.2;
      float sigma = r*scale;
      float a = 1/(sqrt(2*M_PI)*sigma);
      k = a*a*a*exp(-t*t/(2*sigma*sigma));
      k *= scale;
    }
    float a = glm::max(0.0f, glm::dot(p->direction, idata.normal));
    //cout << p.power.r << " " << p.power.g << " " << p.power.b << "\n";
    //cout << b << " " << a << " " << k << "\n";
    l += b * p->power * a * k;
    //l += a;
  }
  l /= photonmap->getTotalPhotons();
  l *= 128.0f; //magic scaling

  return l;
}


vec3 PhotonMapper::getAmbient(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata, int thread_id) {
  const size_t final_gather_samples = 0;
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
    l = getLuminance(incoming_ray, idata);
  }
  return l;
}

vec4 PhotonMapper::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata,
    float attenuation, unsigned short  depth, int thread_id) {
  if(idata.missed()){
    // No intersection
    return settings->background_color;
  }

  vec3 l = getAmbient(incoming_ray, idata, thread_id);
  vec3 color = scene->getMaterialVector()[idata.material]->getDiffuse();
  return vec4(l*color,1);
}


}
