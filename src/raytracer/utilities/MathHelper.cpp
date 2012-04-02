/*
 * MathHelper.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: david
 */

#include "MathHelper.h"

namespace raytracer {

MathHelper::MathHelper() {
  // TODO Auto-generated constructor stub

}

MathHelper::~MathHelper() {
  // TODO Auto-generated destructor stub
}

float MathHelper::lengthOfVector(vec3 v) {
  return (sqrt(v.x*v.x+v.y*v.y+v.z*v.z));
}

}
