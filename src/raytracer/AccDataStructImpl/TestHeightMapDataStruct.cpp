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
  //iluInit();

  ILuint image;

  image = ilGenImage();
  ilBindImage(image);

  ilLoadImage("heightmap2.png");

  if(ilGetError() == IL_NO_ERROR) {
    cout << "Heightmap loaded" << endl;
    ILuint w,h;

    w = ilGetInteger(IL_IMAGE_WIDTH);
    h = ilGetInteger(IL_IMAGE_HEIGHT);

    heightmap = new Heightmap(w,h,vec3(0,0,0),vec3(0,0,1),vec3(1,0,0),1,ilGetData());

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

  return heightmap->getTriangle(ray);

  //return IAccDataStruct::IntersectionData(0,vec3(0,0,0),vec3(0,1,0),vec2(0,0),vec3(0,0,0),vec3(0,0,0));

}
}
