/*
 * CombinedDataStruct.h
 *
 *  Created on: Apr 9, 2012
 *      Author: idk
 */

#ifndef COMBINEDDATASTRUCT_H_
#define COMBINEDDATASTRUCT_H_

#include "../IAccDataStruct.h"

namespace raytracer {

class CombinedDataStruct: public IAccDataStruct {
public:
  CombinedDataStruct();
  virtual ~CombinedDataStruct();

  IntersectionData findClosestIntersection(Ray ray);
  void setData(Triangle** triangles,size_t size,AABB aabb);
  vector<AABB>& getAABBList(){return datastructs[0]->getAABBList();}

private:
  vector<IAccDataStruct*> datastructs;
};

} /* namespace raytracer */
#endif /* COMBINEDDATASTRUCT_H_ */
