/*
 * PhotonMapper.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "PhotonMapper.h"
#include "../PhotonMapImpl/HashPM.h"

namespace raytracer {

PhotonMapper::PhotonMapper(Scene* scene)
: BaseTracer(scene) {
  radius = 0.1;
  photonmap = new HashPM(radius, 1024);

}

PhotonMapper::~PhotonMapper() {
  // TODO Auto-generated destructor stub
  delete photonmap;
}

void PhotonMapper::storeInMap(Photon p){
  photonmap->addItem(p.position, p);
}

bool PhotonMapper::bounce(Photon& p) {
  Ray ray(p.position, -p.direction);
  cout << p.direction.x << " " << p.direction.y << " " << p.direction.z << "\n";

  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  p.position = idata.interPoint;
  p.normal = idata.normal;
  if(idata.material == idata.NOT_FOUND){
    return false;
  }

  vec3 color = scene->getMaterialVector()[idata.material]->getDiffuse();
  p.power = vec4(color,1);
  storeInMap(p);

  return false;

  float absorbtion, reflection, refraction;
  vec3 outgoing;

  assert(absorbtion+reflection+refraction==1.0f);

  //russian roulette
  float rand = drand48();
  if(rand < absorbtion) {
    return false;
  } if (rand < reflection) {
    //set outgoing
  } else if (rand < refraction) {
    //set outgoing
  }

  p.direction = -outgoing;
  p.power *= vec4(color,p.power.a * 0.5); //0.5 is todo

  if(p.power.a < settings->recursion_attenuation_threshold)
    return false;

  return true;
}

void PhotonMapper::getPhotons() {
  size_t n = 1024; //NUMBER_OF_PHOTONS;
  size_t max_recursion_depth = settings->max_recursion_depth;
  float totalpower = 0;
  for(size_t i=0; i<lights->size(); ++i){
    totalpower += lights->at(i)->getPower();
  }

  float power = totalpower / n;
  for(size_t i=0; i<lights->size(); ++i){
    ILight* light = lights->at(i);
    size_t m = n * light->getPower() / totalpower; //photons per light
    Ray* rays = new Ray[m];
    light->getRays(rays, m);

    for(size_t j=0; j<m; ++j){
      Ray ray = rays[i];

      Photon p;
      p.power = vec4(light->getColor(), power);
      p.direction = -ray.getDirection();
      p.position = ray.getPosition();

      for(size_t k=0; k<max_recursion_depth; ++k) {
        if(!bounce(p))
          break;
      }
    }
  }

}

void PhotonMapper::initTracing(){
  BaseTracer::initTracing();
  //make map
  getPhotons();
  //balance map
}


vector<Photon> PhotonMapper::gather(float& r, vec3 point){
  r = radius;
  return photonmap->gatherFromR(point, r);
}

float inline brdf(vec3 point, vec3 idir, vec3 odir){
  return 1.0;
}

vec4 PhotonMapper::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata){
  vec4 l = vec4(0);

  size_t g; //number of photons
  float r;  //gather radius

  vector<Photon> photons = gather(r, idata.interPoint);
  cout << photons.size() << " ";
  for(size_t i=0; i<photons.size(); ++i){
    Photon p = photons[i];
    float b = brdf(idata.interPoint, p.direction, incoming_ray.getDirection());
    float k = 1/(M_PI*r*r); //filter kernel
    float a = glm::max(0.0f, glm::dot(p.direction, idata.normal));
    l += b * p.power * a * k;
  }

  vec3 color = scene->getMaterialVector()[idata.material]->getDiffuse();
  //cout << l.r << " " << l.g << " " << l.b << "\n";
  return l;
}


}
