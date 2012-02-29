/*
 * VertexArrayDataStruct.h
 *
 *  Created on: Feb 3, 2012
 *      Author: ulvinge
 */

#ifndef VERTEXARRAYDATASTRUCT_H_
#define VERTEXARRAYDATASTRUCT_H_

#include <vector>
#include <glm/glm.hpp>
#include <GL/glfw.h>

#include "../IAccDataStruct.h"

#include "../utilities/Ray.h"

#include "../IDraw.h"

using namespace glm;

namespace raytracer {

class Scene;

class VertexArrayDataStruct : public IDraw {
public:
  VertexArrayDataStruct(Scene* scene, std::vector<Triangle*> triangles);
  virtual ~VertexArrayDataStruct();

  GLuint getVertexArrayObject();
  void draw();

private:
  GLuint positionBuffer;
  GLuint colorBuffer;
  GLuint texcoordBuffer;
  GLuint normalBuffer;
  GLuint materialBuffer;
  GLuint vertexArrayObject;
  GLuint m_texture;
  size_t size;

};
}

#endif /* VERTEXARRAYDATASTRUCT_H_ */
