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
  PhotonMapper(Scene* scene, Settings* settings);
  virtual ~PhotonMapper();

private:
  Photon* photons;
  void getPhotons();
  void storeInMap(Photon p);
  bool bounce(Photon& p);
  vec4 shade(Ray incoming_ray, IAccDataStruct::IntersectionData idata);
  Photon* gather(size_t& g, float& r, vec3 point);
  void initTracing();
};

}

#endif /* PHOTONMAPPER_H_ */
