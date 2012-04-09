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

#include <fstream>

#include "IDraw.h"

namespace raytracer {

struct Photon {
  vec3 direction; //pointing away from position, by convention
  vec3 position;
  vec3 normal;
  vec3 power; //color
};

class IPhotonMap : IDraw {
public:
  virtual ~IPhotonMap() {};

  virtual void balance() = 0;
  virtual void addPhoton(Photon p) = 0;
  //virtual void gatherFromG(vec3 point, float r, Photon* p, size_t g) = 0;
  virtual vector<Photon*> gatherFromR(vec3 point, float r) const = 0;
  virtual size_t getTotalPhotons() const = 0;
  virtual size_t getNumberOfBuckets() = 0;
  virtual vector<Photon>& getBucket(size_t t) = 0;

  void write(const char* filename){
    ofstream file;
    file.open (filename);
    for(size_t j=0; j<getNumberOfBuckets(); ++j){
      vector<Photon>& photons = getBucket(j);
      for(size_t i=0; i<photons.size(); ++i){
        const Photon p = photons[i];
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

  void read(const char* filename){
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

  void draw() {
    glBegin(GL_POINTS);
    for(size_t j=0; j<getNumberOfBuckets(); ++j){
      vector<Photon>& photons = getBucket(j);
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
};

}

#endif /* IPHOTONMAP_H_ */
