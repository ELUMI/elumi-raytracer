/*
 * PhotonMapper.h
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#ifndef PHOTONMAPPER_H_
#define PHOTONMAPPER_H_

#include "BaseTracer.h"
#include "../IPhotonMap.h"

namespace raytracer {

class PhotonMapper: public BaseTracer {
public:
  PhotonMapper(Scene* scene);
  virtual ~PhotonMapper();

  IPhotonMap* photonmap;
private:
  Photon* photons;
  void getPhotons();
  void storeInMap(Photon p);
  bool bounce(Photon& p);
  vec4 shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata);
  vector<Photon> gather(float& r, vec3 point);
  void initTracing();

  float radius;
  size_t totalphotons;
};

}

#endif /* PHOTONMAPPER_H_ */
