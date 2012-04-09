/*
 * HashPM.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "HashPM.h"

namespace raytracer {

HashPM::HashPM(float bucketsize, size_t n_buckets)
  : hashpoint(bucketsize, n_buckets), totalPhotons(0) {
}

HashPM::~HashPM() {
}

void HashPM::addPhoton(Photon p){
  hashpoint.addItem(p.position, p);
  ++totalPhotons;
}

size_t HashPM::getTotalPhotons() const {
  return totalPhotons;
}
size_t HashPM::getNumberOfBuckets(){
  return hashpoint.getNumberOfBuckets();
}
vector<Photon>& HashPM::getBucket(size_t t){
  return hashpoint.getBucket(t);
}

vector<Photon*> HashPM::gatherFromR(vec3 point, float r) const {
  return hashpoint.gatherFromR(point, r);
}

void HashPM::balance(){
}


}
