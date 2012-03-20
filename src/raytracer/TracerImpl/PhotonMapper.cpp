/*
 * PhotonMapper.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "PhotonMapper.h"

namespace raytracer {

PhotonMapper::PhotonMapper(Scene* scene, Settings* settings)
: BaseTracer(scene, settings) {

}

PhotonMapper::~PhotonMapper() {
  // TODO Auto-generated destructor stub
}

void PhotonMapper::storeInMap(Photon p){

}


void PhotonMapper::bounce(Photon& p) {
  Ray ray(p.position, -p.incident_direction);
  IAccDataStruct::IntersectionData idata = datastruct->findClosestIntersection(ray);
  p.position = idata.interPoint;
  p.normal = idata.normal;

  storeInMap(p);


  float absorbtion, reflection, refraction;
  vec3 outgoing;

  //russian roulette
  float rand = drand48();
  if(rand < absorbtion) {
    p.incident_power.a = 0.0f;
  } if (rand < reflection) {
    //set outgoing
  } else if (rand < refraction) {
    //set outgoing
  }

  p.incident_direction = -outgoing;
  p.incident_power.a *= 0.5; //todo calc
}

void PhotonMapper::getPhotons() {
  size_t n = 1024; //NUMBER_OF_PHOTONS;
  float totalpower = 0;
  for(size_t i; i<lights->size(); ++i){
    totalpower += lights->at(i)->getPower();
  }

  float power = totalpower / n;
  for(size_t i=0; i<lights->size(); ++i){
    ILight* light = lights->at(i);
    size_t m = n * light->getPower() / totalpower;
    Ray* rays = new Ray[m];
    light->getRays(rays, m);

    for(size_t j=0; j<m; ++j){
      Ray ray = rays[i];

      Photon p;
      p.incident_power = vec4(light->getColor(), power);
      p.incident_direction = -ray.getDirection();
      p.position = ray.getPosition();

      for(size_t k=0; k<3; ++k) {
        bounce(p);

        if(p.incident_power.a < 0.001)
          break;
      }
    }
  }

}

void PhotonMapper::initTracing(){
  //make map
  getPhotons();
  //balance map
}


Photon* PhotonMapper::gather(size_t& g, float& r, vec3 point){

}

float brdf(vec3 point, vec3 idir, vec3 odir){

}

float inline kernel(vec3 d, float r){
  return 1/(M_PI*r*r);
}

vec4 PhotonMapper::shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata){
  vec4 l = vec4(0);

  size_t g; //number of photons
  float r;  //gather radius

  Photon* photons = gather(g, r, idata.interPoint);
  for(size_t i=0; i<g; ++g){
    Photon p = photons[i];
    float b = brdf(idata.interPoint, p.incident_direction, incoming_ray.getDirection());
    float k = kernel(p.position - idata.interPoint, r);
    float a = glm::max(0.0f, glm::dot(p.incident_direction, idata.normal));
    l += b * p.incident_power * a * k;
  }
}


}
