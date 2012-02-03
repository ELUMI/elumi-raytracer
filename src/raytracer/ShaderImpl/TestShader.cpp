/*
 * TestShader.cpp
 *
 *  Created on: Feb 3, 2012
 *      Author: irri
 */

#include "TestShader.h"

namespace raytracer {

TestShader::TestShader() {

}

TestShader::~TestShader() {

}

glm::vec4 TestShader::shade(Ray* ray, IAccDataStruct::IntersectionData intersection_data) {
  return vec4(1,1,1,1);
}

}
