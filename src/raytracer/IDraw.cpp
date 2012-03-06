/*
 * IDraw.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: ulvinge
 */

#include <GL/glew.h>

#include "IDraw.h"

#include <glm/gtc/matrix_transform.hpp> //translate
#include <glm/gtc/type_ptr.hpp> //value_ptr

#include <GL/glfw.h>

namespace raytracer {

IDraw::IDraw() {
}

IDraw::~IDraw() {
}

void IDraw::drawWithView(mat4 view, GLuint loc){
  view = glm::translate(view, this->getPosition());
  glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(view));
  draw();
}

void IDraw::drawWithGLView(mat4 view){
  glMatrixMode(GL_MODELVIEW);
  view = glm::translate(view, this->getPosition());
  glPushMatrix();
  glLoadMatrixf(value_ptr(view));
  draw();
  glLoadIdentity();
  glPopMatrix();
}

void IDraw::draw(){
}

vec3 IDraw::getPosition(){
  return vec3(0,0,0);
}

}
