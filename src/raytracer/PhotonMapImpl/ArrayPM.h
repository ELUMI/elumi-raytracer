/*
 * ArrayPM.h
 *
 *  Created on: Apr 9, 2012
 *      Author: idk
 */

#ifndef ARRAYPM_H_
#define ARRAYPM_H_

#include "../IPhotonMap.h"

namespace raytracer {

class ArrayPM: public IPhotonMap {
public:
  ArrayPM(size_t size=0);
  virtual ~ArrayPM();

  void balance();
  void addPhoton(Photon p);
  vector<Photon*> gatherFromR(vec3 point, float r) const;
  size_t getTotalPhotons() const;
  size_t getNumberOfBuckets();
  vector<Photon>& getBucket(size_t t);

  Photon* getBucket(vec3 point);

  void write(const char* filename);
  void read(const char* filename);

private:
  vector<Photon> array;
};

} /* namespace raytracer */
#endif /* ARRAYPM_H_ */
