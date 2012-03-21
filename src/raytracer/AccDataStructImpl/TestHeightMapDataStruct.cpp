/*
 * TestHeightMapDataStruct.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: david
 */

#include "TestHeightMapDataStruct.h"

namespace raytracer {

TestHeightMapDataStruct::TestHeightMapDataStruct() {
  ilInit();
  iluInit();

  ILuint image;

  image = ilGenImage();
  ilBindImage(image);

  ilLoadImage("heightmap.png");

  if(ilGetError() == IL_NO_ERROR) {
    cout << "Heightmap loaded" << endl;
    ILuint w,h;

    w = ilGetInteger(IL_IMAGE_WIDTH);
    h = ilGetInteger(IL_IMAGE_HEIGHT);

    heightmap = new Heightmap(w,h,300,300,vec3(0,0,0),ilGetData());

  } else {
    //Image not loaded (?)
    cout << "Heigtmap not loaded" << endl;
  }

}

TestHeightMapDataStruct::~TestHeightMapDataStruct() {
  // TODO Auto-generated destructor stub
}

IAccDataStruct::IntersectionData
TestHeightMapDataStruct::findClosestIntersection(Ray ray) {
  return IAccDataStruct::IntersectionData();
}

void TestHeightMapDataStruct::setData(std::vector<Triangle*> new_triangles) {}

}
