/*
 * HashPM.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "HashPM.h"
#include <set>

namespace raytracer {

HashPM::HashPM(float bucketsize, size_t n_buckets) {
  this->bucketsize = bucketsize;
  this->n_buckets = n_buckets;

  buckets = new vector<Photon>[n_buckets];
}

HashPM::~HashPM() {
  delete [] buckets;
}

size_t HashPM::hash(vec3 point){
  size_t h = int(point.x/bucketsize)*17;
  h += int(point.y/bucketsize)*101;
  h += int(point.z/bucketsize)*67;
  return h % n_buckets;
}

void HashPM::balance(){
}

void HashPM::addItem(vec3 point, Photon i){
  size_t pos = hash(point);
  buckets[pos].push_back(i);
}


void HashPM::gatherFromG(vec3 point, float r, Photon* p, size_t g){
}

vector<Photon> HashPM::gatherFromR(vec3 point, float r){
  set<size_t> positions;
  for(float x=point.x-r; x<point.x+r+bucketsize; x+=bucketsize){
    for(float y=point.y-r; y<point.y+r+bucketsize; y+=bucketsize){
      for(float z=point.z-r; z<point.z+r+bucketsize; z+=bucketsize){
        positions.insert(hash(vec3(x,y,z)));
      }
    }
  }

  vector<Photon> found;
  for(set<size_t>::iterator pos=positions.begin(); pos != positions.end(); ++pos){
    for(size_t i=0; i<buckets[*pos].size(); ++i){
      Photon p = buckets[*pos][i];
      if(length(p.position-point) < r) {
        found.push_back(p);
      }
    }
  }
  return found;
}

}
