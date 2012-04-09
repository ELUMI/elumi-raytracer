/*
 * ArrayPM.cpp
 *
 *  Created on: Apr 9, 2012
 *      Author: idk
 */

#include "ArrayPM.h"

namespace raytracer {

ArrayPM::ArrayPM(size_t size) {
  array.reserve(size);
}

ArrayPM::~ArrayPM() {
}

void ArrayPM::balance(){
}
void ArrayPM::addPhoton(Photon p){
  array.push_back(p);
}

vector<Photon*> ArrayPM::gatherFromR(vec3 point, float r) const {
  vector<Photon*> found;
  for(size_t i=0; i<array.size(); ++i){
    Photon* p = const_cast<Photon*>(&(array[i]));
    if(length(p->position-point) < r) {
      found.push_back(p);
    }
  }
  return found;
}

size_t ArrayPM::getTotalPhotons() const {
  return array.size();
}

size_t ArrayPM::getNumberOfBuckets(){
  return 1;
}

vector<Photon>& ArrayPM::getBucket(size_t t){
  return array;
}


} /* namespace raytracer */
