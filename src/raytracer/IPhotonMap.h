/*
 * IPhotonMap.h
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */


#ifndef IPHOTONMAP_H_
#define IPHOTONMAP_H_

#include <vector>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

#include "IDraw.h"

namespace raytracer {

struct Photon {
  vec3 direction; //pointing away from position, by convention
  vec3 position;
  vec3 normal;
  vec4 power; //color
};

class IPhotonMap : IDraw {
public:
  virtual ~IPhotonMap() {};

  virtual void balance() = 0;
  virtual void addItem(vec3 point, Photon p) = 0;
  virtual void gatherFromG(vec3 point, float r, Photon* p, size_t g) = 0;
  virtual vector<Photon> gatherFromR(vec3 point, float r) = 0;
  virtual void draw() = 0;
};

}

#endif /* IPHOTONMAP_H_ */
