/*
 * TriangleArray.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: ulvinge
 */

#include "TriangleArray.h"
#include <GL/glfw.h>
#include "../scene/Scene.h"

namespace raytracer {

TriangleArray::TriangleArray(Scene* scene, std::vector<Triangle*> triangles) {
  this->triangles = triangles;
  this->scene = scene;

}

TriangleArray::~TriangleArray() {
  // TODO Auto-generated destructor stub
}

void TriangleArray::draw(){
  glBegin(GL_TRIANGLES);
  for(size_t i=0; i<triangles.size(); ++i) {
      Material* mat = scene->getMaterialVector()[triangles[i]->getMaterial()];
      vec3 c = mat->getDiffuse();
      glColor3f(c.r, c.g, c.b);

      vector <vec3*> vs = triangles[i]->getVertices();
      glVertex3f(vs[0]->x, vs[0]->y, vs[0]->z);
      glVertex3f(vs[1]->x, vs[1]->y, vs[1]->z);
      glVertex3f(vs[2]->x, vs[2]->y, vs[2]->z);
  }
  glEnd();

}


}
