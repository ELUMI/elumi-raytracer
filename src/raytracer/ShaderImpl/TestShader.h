/*
 * TestShader.h
 *
 *  Created on: Feb 3, 2012
 *      Author: irri
 */

#ifndef TESTSHADER_H_
#define TESTSHADER_H_

#include "../IShader.h"

namespace raytracer {

class TestShader: public raytracer::IShader {
public:
  TestShader();
  virtual ~TestShader();

  glm::vec4 shade(Ray* ray, IAccDataStruct::IntersectionData intersection_data);
};

}

#endif /* TESTSHADER_H_ */
