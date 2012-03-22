/*
 * HashPM.h
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#ifndef HASHPM_H_
#define HASHPM_H_

#include "../IPhotonMap.h"
#include <vector>

using namespace std;

namespace raytracer {

class HashPM : public IPhotonMap {
public:
  HashPM(float bucketsize, size_t n_buckets);
  virtual ~HashPM();

  void balance();
  void addItem(vec3 point, Photon p);
  void gatherFromG(vec3 point, float r, Photon* p, size_t g);
  vector<Photon> gatherFromR(vec3 point, float r);

  Photon* getBucket(vec3 point);

protected:
  virtual size_t hash(vec3 point);

  float bucketsize;
  size_t n_buckets;

private:
  vector<Photon>* buckets;
};

}

#endif /* HASHPM_H_ */
