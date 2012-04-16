/*
 * OBB.h
 *
 *  Created on: Mar 26, 2012
 *      Author: david
 */

#ifndef OBB_H_
#define OBB_H_

#include "Ray.h"
#include "glm/glm.hpp"

using namespace glm;

namespace raytracer {

class OBB {
public:

  struct IntersectionData {
    IntersectionData();
    IntersectionData(bool intersected, vec3 position) {
      IntersectionData::intersected = intersected;
      IntersectionData::position = position;
    }
    vec3 position;
    bool intersected;
  };

  // TODO: Ugly temp solution
  struct IntervalData {
    IntervalData();
    IntervalData(bool intersected, bool inside, vec3 min, vec3 max) {
      IntervalData::min = min;
      IntervalData::max = max;
      IntervalData::intersected = intersected;
      IntervalData::inside = inside;
    }
    vec3 min;
    vec3 max;
    bool intersected;
    bool inside;  // The ray starts inside the obb and only intersects one wall.
  };

  OBB();
  OBB(vec3 position, vec3 u, vec3 v, vec3 w);
  virtual ~OBB();
  IntersectionData rayIntersection(Ray ray);
  IntervalData rayInterval(Ray ray);
private:
  vec3 position,u,v,w,u_n,v_n,w_n;
};

}

#endif /* OBB_H_ */
