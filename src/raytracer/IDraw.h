/*
 * IDraw.h
 *
 *  Created on: Feb 22, 2012
 *      Author: ulvinge
 */

#ifndef IDRAW_H_
#define IDRAW_H_

#include <glm/glm.hpp>
#include <GL/glfw.h>

using namespace glm;

namespace raytracer {

class IDraw {
public:
  IDraw();
  virtual ~IDraw();

  virtual void drawWithView(mat4 view, GLuint loc);
  virtual void draw();

  virtual vec3 getPosition();

};

}
#endif /* IDRAW_H_ */
