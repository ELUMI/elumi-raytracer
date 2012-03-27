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
  bool bounce(Photon& p);
  vector<Photon*> gather(float& r, vec3 point);
  virtual vec4 shade(Ray ray, IAccDataStruct::IntersectionData idata, float attenuation, unsigned short depth);
  virtual void initTracing();
  vec3 getLuminance(Ray incoming_ray, IAccDataStruct::IntersectionData idata);

  float radius;
};

}

#endif /* PHOTONMAPPER_H_ */
