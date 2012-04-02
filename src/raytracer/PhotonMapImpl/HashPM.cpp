/*
 * HashPM.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#include "HashPM.h"
#include <fstream>

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

vector<Photon*> HashPM::gatherFromR(vec3 point, float r) const {
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
      vec3 c = p.power;
      //c = vec4(0,1,0,0);
      //c = glm::normalize(c);
      vec3 v = p.position;
      glColor3f(c.r, c.b, c.g);
      glVertex3f(v.x, v.y, v.z);
    }
  }
  glEnd();
}

void HashPM::write(const char* filename){
  ofstream file;
  file.open (filename);
  for(size_t j=0; j<hashpoint.getNumberOfBuckets(); ++j){
    vector<Photon> photons = hashpoint.getBucket(j);
    for(size_t i=0; i<photons.size(); ++i){
      Photon p = photons[i];
      vec3 c = p.position;
      file << c.x << "\t" << c.y << "\t" << c.z << "\t";
      c = p.direction;
      file << c.x << "\t" << c.y << "\t" << c.z << "\t";
      c = p.normal;
      file << c.x << "\t" << c.y << "\t" << c.z << "\t";
      c = p.power;
      file << c.x << "\t" << c.y << "\t" << c.z << "\t";
    }
  }
  file.close();
}

void HashPM::read(const char* filename){
  ifstream file;
  file.open (filename);

  while(!file.eof()) {
    Photon p;
    vec3 c;
    file >> c.x >> c.y >> c.z;
    p.position = c;
    file >> c.x >> c.y >> c.z;
    p.direction = c;
    file >> c.x >> c.y >> c.z;
    p.normal = c;
    file >> c.x >> c.y >> c.z;
    p.power = c;

    addPhoton(p);
  }
  file.close();
}


}
