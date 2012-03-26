/*
 * HashPoint.h
 *
 *  Created on: Mar 23, 2012
 *      Author: ulvinge
 */

#ifndef HASHPOINT_H_
#define HASHPOINT_H_

#include <vector>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

#include <set>

namespace raytracer {

template <class T>
class HashPoint {
public:
  HashPoint(float bucketsize, size_t n_buckets);
  virtual ~HashPoint();

  void addItem(vec3 point, T p);
  void addItem(size_t hash, T p);
  //void addGrid(vec3 min, vec3 max, T p);
  vector<T> gatherFromR(vec3 point, float r);

  size_t getNumberOfBuckets();
  float getBucketSize();

  //T* getBucket(vec3 point);
  vector<T> getBucket(size_t hash);
  vector<T> getBucket(vec3 point);

  virtual size_t hash(vec3 point);
protected:

  float bucketsize;
  size_t n_buckets;

private:
  vector<T>* buckets;
};


template <class T>
HashPoint<T>::HashPoint(float bucketsize, size_t n_buckets) {
  this->bucketsize = bucketsize;
  this->n_buckets = n_buckets;

  buckets = new vector<T>[n_buckets];
}

template <class T>
HashPoint<T>::~HashPoint() {
  delete [] buckets;
}

template <class T>
size_t HashPoint<T>::hash(vec3 point){
  size_t h = int(point.x/bucketsize)*17;
  h += int(point.y/bucketsize)*101;
  h += int(point.z/bucketsize)*67;
  return h % n_buckets;
}

template <class T>
void HashPoint<T>::addItem(vec3 point, T i){
  size_t pos = hash(point);
  buckets[pos].push_back(i);
}

template <class T>
void HashPoint<T>::addItem(size_t pos, T i){
  buckets[pos].push_back(i);
}


template <class T>
vector<T> HashPoint<T>::gatherFromR(vec3 point, float r){
  set<size_t> positions;
  for(float x=point.x-r; x<point.x+r+bucketsize; x+=bucketsize){
    for(float y=point.y-r; y<point.y+r+bucketsize; y+=bucketsize){
      for(float z=point.z-r; z<point.z+r+bucketsize; z+=bucketsize){
        positions.insert(hash(vec3(x,y,z)));
      }
    }
  }

  vector<T> found;
  for(set<size_t>::iterator pos=positions.begin(); pos != positions.end(); ++pos){
    for(size_t i=0; i<buckets[*pos].size(); ++i){
      T p = buckets[*pos][i];
      if(length(p.position-point) < r) {
        found.push_back(p);
      }
    }
  }
  return found;
}

template <class T>
size_t HashPoint<T>::getNumberOfBuckets(){
  return n_buckets;
}

template <class T>
vector<T> HashPoint<T>::getBucket(size_t hash){
  return buckets[hash];
}

template <class T>
vector<T> HashPoint<T>::getBucket(vec3 point){
  return buckets[hash(point)];
}

template <class T>
float HashPoint<T>::getBucketSize(){
  return bucketsize;
}

}

#endif /* HASHPOINT_H_ */
