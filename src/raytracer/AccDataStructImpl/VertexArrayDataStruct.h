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
#include <GL/gl.h>

#include "../IAccDataStruct.h"

#include "../utilities/Ray.h"

using namespace glm;

namespace raytracer {

class VertexArrayDataStruct: public raytracer::IAccDataStruct {
public:
  VertexArrayDataStruct();
  virtual ~VertexArrayDataStruct();

  IntersectionData findClosestIntersection(Ray ray);
  void setData(std::vector<Triangle*> triangles);
  GLuint getVertexArrayObject();
  void draw();

private:
  vector<vec3> m_vertices;
  vector<vec3> m_colors;
  vector<vec2> m_texcoords;
  vector<int> m_indices;
  GLuint positionBuffer;
  GLuint colorBuffer;
  GLuint texcoordBuffer;
  GLuint indexBuffer;
  GLuint vertexArrayObject;
  GLuint m_texture;

};
}

#endif /* VERTEXARRAYDATASTRUCT_H_ */
