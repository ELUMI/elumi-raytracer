/*
 * HashPM.h
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#ifndef HASHPM_H_
#define HASHPM_H_

#include "../IPhotonMap.h"
#include "../utilities/HashPoint.h"
#include <vector>

using namespace std;

namespace raytracer {

class HashPM : public IPhotonMap {
public:
  HashPM(float bucketsize, size_t n_buckets);
  virtual ~HashPM();

  void balance();
  void addPhoton(Photon p);
  //void gatherFromG(vec3 point, float r, Photon* p, size_t g);
  vector<Photon*> gatherFromR(vec3 point, float r) const;
  void draw();

  Photon* getBucket(vec3 point);

  void write(const char* filename);
  void read(const char* filename);

private:
  HashPoint<Photon> hashpoint;
};

}

#endif /* HASHPM_H_ */
