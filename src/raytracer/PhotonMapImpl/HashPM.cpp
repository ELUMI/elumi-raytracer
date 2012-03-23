/*
 * HashPM.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "HashPM.h"

namespace raytracer {

HashPM::HashPM(float bucketsize, size_t n_buckets)
  : hashpoint(bucketsize, n_buckets) {

}

HashPM::~HashPM() {
}

void HashPM::addPhoton(Photon p){
  hashpoint.addItem(p.position, p);
}

vector<Photon> HashPM::gatherFromR(vec3 point, float r){
  return hashpoint.gatherFromR(point, r);
}

void HashPM::balance(){
}

void HashPM::draw(){
  glBegin(GL_POINTS);
  for(size_t j=0; j<hashpoint.getNumberOfBuckets(); ++j){
    vector<Photon> photons = hashpoint.getBucket(j);
    for(size_t i=0; i<photons.size(); ++i){
      Photon p = photons[i];
      vec4 c = p.power;
      c = vec4(0,1,0,0);
      vec3 v = p.position;
      glColor3f(c.r, c.b, c.g);
      glVertex3f(v.x, v.y, v.z);
    }
  }
  glEnd();
}


}
