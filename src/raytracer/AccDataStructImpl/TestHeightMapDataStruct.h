/*
 * TestHeightMapDataStruct.h
 *
 *  Created on: Mar 21, 2012
 *      Author: david
 */

#ifndef TESTHEIGHTMAPDATASTRUCT_H_
#define TESTHEIGHTMAPDATASTRUCT_H_

#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"
#include "../IAccDataStruct.h"
#include "../scene/Heightmap.h"

#include <IL/il.h>
#include <IL/ilu.h>

#include <vector>

namespace raytracer {

class TestHeightMapDataStruct : public IAccDataStruct {
public:
  TestHeightMapDataStruct();
  virtual ~TestHeightMapDataStruct();
  IAccDataStruct::IntersectionData findClosestIntersection(Ray ray);
  void setData(Triangle** new_triangles,size_t size,AABB* aabb){};
  vector<AABB>& getAABBList(){return aabb_list;}
private:
  Heightmap* heightmap;
  vector<AABB> aabb_list;
};

}

#endif /* TESTHEIGHTMAPDATASTRUCT_H_ */
