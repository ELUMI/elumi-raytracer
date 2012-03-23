/*
 * TriangleArray.h
 *
 *  Created on: Feb 28, 2012
 *      Author: ulvinge
 */

#ifndef TRIANGLEARRAY_H_
#define TRIANGLEARRAY_H_

#include <vector>
#include <glm/glm.hpp>
#include <GL/glfw.h>

#include "../IAccDataStruct.h"
#include "../utilities/Ray.h"
#include "../utilities/Triangle.h"
#include "../IDraw.h"

using namespace glm;

namespace raytracer {

class Scene;

class TriangleArray: public IDraw {
public:
  TriangleArray(Scene* scene, std::vector<Triangle*> triangles);
  virtual ~TriangleArray();

  void draw();

private:
  std::vector<Triangle*> triangles;
  Scene* scene;
};

}

#endif /* TRIANGLEARRAY_H_ */
