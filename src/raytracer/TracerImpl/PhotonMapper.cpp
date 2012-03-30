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
  radius = 1;
  photonmap = new HashPM(radius, 1024*128);
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

bool PhotonMapper::bounce(Photon& p) {
  Ray ray(p.position, -p.direction);
  //cout << p.direction.x << " " << p.direction.y << " " << p.direction.z << "\n";

  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  p.position = idata.interPoint;
  p.normal = idata.normal;
  if(idata.material == idata.NOT_FOUND){
    return false;
  }

  storeInMap(p);

  //return false;

  Material* mat = scene->getMaterialVector()[idata.material];

  float reflection = mat->getReflection()/3;
  float refraction = mat->getReflection()/3;
  float absorbtion = 1/3;
  float diffuse    = 1 - reflection - refraction - absorbtion;
  vec3 outgoing;

  assert(reflection <= 1 && refraction <= 1 && absorbtion <= 1 && diffuse <= 1);
  assert(reflection+refraction+diffuse+absorbtion==1.0f);

  //russian roulette
  float rand = gen_random_float(0,1);
  if (rand < reflection) {
    outgoing = glm::reflect(p.direction, p.normal);
    p.power *= reflection * mat->getDiffuse();
  } else if (rand < reflection+refraction) {
    outgoing = glm::refract(p.direction, p.normal, mat->getIndexOfRefraction());
    p.power *= refraction * mat->getDiffuse();
  } else if(rand < reflection+refraction+diffuse) {  //diffuse interreflection
    outgoing = get_random_hemisphere(p.normal);
    p.power *= diffuse    * mat->getDiffuse();
  } else { //absorbtion
    return false;
  }

  p.direction = -outgoing;

  if((p.power.r + p.power.g + p.power.b)/3 < settings->recursion_attenuation_threshold)
    return false;

  return true;
}

void PhotonMapper::getPhotons() {
  size_t n = 1024*32; //NUMBER_OF_PHOTONS;
  size_t max_recursion_depth = settings->max_recursion_depth;
  float totalpower = 0;
  for(size_t i=0; i<lights->size(); ++i){
    totalpower += lights->at(i)->getPower();
  }

  for(size_t i=0; i<lights->size(); ++i){
    ILight* light = lights->at(i);
    float npe = n * light->getPower() / totalpower;
    size_t m = size_t(npe); //photons per light
    Ray* rays = new Ray[m];
    light->getRays(rays, m);

    vec3 power = (1/(light->getPower() / totalpower)) * light->getColor() * light->getIntensity();

    for(size_t j=0; j<m; ++j){
      Ray ray = rays[j];

      Photon p;
      p.power = power;
      p.direction = -ray.getDirection();
      p.position = ray.getPosition();

      for(size_t k=0; k<max_recursion_depth; ++k) {
        if(abort)
          return;
        if(!bounce(p))
          break;
      }
    }
  }
}

vector<Photon*> PhotonMapper::gather(float& r, vec3 point){
  r = radius;
  return photonmap->gatherFromR(point, r);
}

float inline brdf(vec3 point, vec3 idir, vec3 odir){
  return 1.0;
}

vec3 PhotonMapper::getLuminance(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata) {
  vec3 l = vec3(0);

  size_t g; //number of photons
  float r;  //gather radius

  vector<Photon*> photons = gather(r, idata.interPoint);
  if(photons.size() == 0)
    return vec3(0);
  for(size_t i=0; i<photons.size(); ++i){
    Photon* p = photons[i];
    float b = brdf(idata.interPoint, p->direction, incoming_ray.getDirection());

    float k;
    //k = 1/(M_PI*r*r); //simple filter kernel

    { //advanced filter kernel (ISPM paper)
      float dist = length(idata.interPoint-p->position);
      float rz = 0.1 * r;
      float t = (dist/r)*(1-dot((idata.interPoint-p->position) / dist, p->normal)*(r+rz)/rz);
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

  return l;
}


vec4 PhotonMapper::shade(Ray incoming_ray,
    IAccDataStruct::IntersectionData idata,
    float attenuation, unsigned short depth) {
  if (idata.material == IAccDataStruct::IntersectionData::NOT_FOUND) {
    // No intersection
    return settings->background_color;
  }

  vec3 l=vec3(0);
  if(false) { //final gather
    l=vec3(0);
    const size_t final_gather_samples = 8;
    for(size_t i=0; i<final_gather_samples; ++i){
      Ray ray = Ray(idata.interPoint, get_random_hemisphere(idata.normal));
      IAccDataStruct::IntersectionData idata2 = datastruct->findClosestIntersection(ray);
      l += getLuminance(ray, idata2);
    }
    l /= final_gather_samples;
  } else {
    l = getLuminance(incoming_ray, idata);
  }

  vec3 color = scene->getMaterialVector()[idata.material]->getDiffuse();
  return vec4(l*color,0);
}


}
