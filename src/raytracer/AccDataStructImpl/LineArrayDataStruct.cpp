/*
 * LineArrayDataStruct.cpp
 *
 *  Created on: Feb 23, 2012
 *      Author: DmZ
 */

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "../common.hpp"
#include "LineArrayDataStruct.hpp"

namespace raytracer{

LineArrayDataStruct::LineArrayDataStruct(std::vector<Line> lines){

  vector<vec3> m_lines;
  for(size_t l=0;l<lines.size();l++){
    Line line = lines[l];
    m_lines.push_back((line.getStart()));
    m_lines.push_back((line.getEnd()));
  }

  glGenBuffers(1, &linesBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, linesBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(glm::vec3),
      &m_lines[0].x, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, linesBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, false/*normalized*/, 0/*stride*/, 0/*offset*/);
  glEnableVertexAttribArray(0);

  size = m_lines.size();
}

LineArrayDataStruct::~LineArrayDataStruct(){

  glDeleteBuffers(1, &linesBuffer);
  glDeleteVertexArrays(1, &vertexArrayObject);
}

void LineArrayDataStruct::draw(){
  glBindVertexArray(vertexArrayObject);
  glDrawArrays(GL_LINES, 0, size);
}

}

