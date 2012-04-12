/*
 * PhotonMapper.h
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#ifndef PHOTONMAPPER_H_
#define PHOTONMAPPER_H_

#include "StandardTracer.h"
#include "../IPhotonMap.h"
#include "../GLData/PhotonProcesser.h"

namespace raytracer {

class PhotonMapper: public StandardTracer {
public:
  PhotonMapper(Scene* scene);
  virtual ~PhotonMapper();

  IPhotonMap* photonmap;
protected:
  Photon* photons;
  void getPhotons();
  void storeInMap(Photon p);
  bool bounce(Photon& p, int thread_id, bool store=true);
  virtual void tracePhoton(Photon p, int thread_id);

  virtual void initTracing();
  virtual void first_bounce();
  vector<Photon*> gather(float& r, vec3 point);
  virtual vec4 shade(Ray ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth, int thread_id=-1);
  vec3 getLuminance(Ray incoming_ray, IAccDataStruct::IntersectionData idata);
  virtual vec3 getAmbient(Ray incoming_ray, IAccDataStruct::IntersectionData idata, int thread_id);
  float filterKernel(vec3 interPoint, vec3 normal, const Photon* p, float r);

  PhotonProcesser* photon_processer;


};

}

#endif /* PHOTONMAPPER_H_ */
