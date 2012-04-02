/*
 * OBB.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: david
 */

#include "OBB.h"

#include <iostream>
#include "MathHelper.h"
using namespace std;


namespace raytracer {

OBB::OBB() {}

OBB::OBB(vec3 position, vec3 u, vec3 v, vec3 w) {
  OBB::position = position;
  OBB::u = u;
  OBB::v = v;
  OBB::w = w;
  OBB::u_n = normalize(u);
  OBB::v_n = normalize(v);
  OBB::w_n = normalize(w);
}

OBB::~OBB() {}

OBB::IntersectionData OBB::rayIntersection(Ray ray) {

  float t_min = -INFINITY;
  float t_max = INFINITY;

  vec3 p = position - ray.getPosition();
  vec3 d = ray.getDirection();
  vec3 o = ray.getPosition();
  vec3 a = u_n;
  vec3 h = u;

  for(int i = 0; i<3 ; i++) {
    switch (i) {
      case 0:
        a = u_n;
        h = u;
        break;
      case 1:
        a = v_n;
        h = v;
        break;
      case 2:
        a = w_n;
        h = w;
        break;
      default:
        break;
    }

    float e = dot(a,p);
    float f = dot(a,d);

    //cout << "h: " << h.length() << endl;

    MathHelper mh = MathHelper();

    if(abs(f) > 0.00000000000001) {
      float t1 = (e+mh.lengthOfVector(h))/f;
      float t2 = (e-mh.lengthOfVector(h))/f;
      if(t1 > t2) {
        float tmp = t1;
        t1 = t2;
        t2 = tmp;
      }
      if(t1 > t_min)
        t_min = t1;
      if(t2 < t_max)
        t_max = t2;
      if(t_min > t_max)
        return IntersectionData(false,vec3());
      if(t_max < 0)
        return IntersectionData(false,vec3());

    } else if(-e-mh.lengthOfVector(h) > 0 ||
        -e+mh.lengthOfVector(h) < 0)
      return IntersectionData(false,vec3());
  }
  if(t_min > 0)
    return IntersectionData(true,o+t_min*d);
  else return IntersectionData(true,o+t_max*d);
}

}
