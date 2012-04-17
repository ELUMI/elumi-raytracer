/*
 * VertexArrayDataStruct.cpp
 *
 *  Created on: Feb 3, 2012
 *      Author: ulvinge
 */

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "VertexArrayDataStruct.h"
#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"
#include "../utilities/glutil.h"
#include "../scene/Scene.h"

using namespace glm;

namespace raytracer {

VertexArrayDataStruct::VertexArrayDataStruct(Scene* scene, std::vector<Triangle*> triangles) {
  vector<vec3> m_vertices;
  vector<vec3> m_colors;
  vector<vec2> m_texcoords;
  vector<vec3> m_normals;
  vector<float> m_material;

  for (unsigned int i = 0; i < triangles.size(); ++i) {
    //int base = m_vertices.size();

    vector<vec3*> vs = triangles[i]->getVertices();
    m_vertices.push_back(*(vs[0]));
    m_vertices.push_back(*(vs[1]));
    m_vertices.push_back(*(vs[2]));

    vs = triangles[i]->getTextures();
    m_texcoords.push_back(vec2(vs[0]->x, vs[0]->y));
    m_texcoords.push_back(vec2(vs[1]->x, vs[1]->y));
    m_texcoords.push_back(vec2(vs[2]->x, vs[2]->y));

    vs = triangles[i]->getNormals();
    m_normals.push_back(*(vs[0]));
    m_normals.push_back(*(vs[1]));
    m_normals.push_back(*(vs[2]));

    std::vector<raytracer::Material*> materials = scene->getMaterialVector();
    unsigned int material_index = triangles[i]->getMaterial();
    m_material.push_back(float(material_index));
    m_material.push_back(float(material_index));
    m_material.push_back(float(material_index));

    vec3 c = materials[material_index]->getDiffuse();
    m_colors.push_back(c);
    m_colors.push_back(c);
    m_colors.push_back(c);
  }

  glGenBuffers(1, &positionBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec3),
      &m_vertices[0].x, GL_STATIC_DRAW);
  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(vec3),
      &m_colors[0].x, GL_STATIC_DRAW);
  glGenBuffers(1, &texcoordBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_texcoords.size() * sizeof(vec2),
      &m_texcoords[0].x, GL_STATIC_DRAW);
  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(vec3),
      &m_normals[0].x, GL_STATIC_DRAW);
  glGenBuffers(1, &materialBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, materialBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_material.size() * sizeof(size_t),
      &m_material[0], GL_STATIC_DRAW);


  //    glGenBuffers( 1, &indexBuffer );
  //    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
  //    glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int), &m_indices[0], GL_STATIC_DRAW );
  CHECK_GL_ERROR();
  // Create the vertex array object
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, false/*normalized*/, 0/*stride*/, 0/*offset*/);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, false/*normalized*/, 0/*stride*/, 0/*offset*/);
  glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
  glVertexAttribPointer(2, 2, GL_FLOAT, false/*normalized*/, 0/*stride*/, 0/*offset*/);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(3, 3, GL_FLOAT, false/*normalized*/, 0/*stride*/, 0/*offset*/);
  glBindBuffer(GL_ARRAY_BUFFER, materialBuffer);
  glVertexAttribPointer(4, 1, GL_FLOAT, false/*normalized*/, 0/*stride*/, 0/*offset*/);
  //  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  // Load texture
  //  m_texture = ilutGLLoadImage("Crate.bmp");
  //  glActiveTexture(GL_TEXTURE0);
  //  glBindTexture(GL_TEXTURE_2D, m_texture);
  //  glGenerateMipmap(GL_TEXTURE_2D);
  //  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  size = m_vertices.size();
}

VertexArrayDataStruct::~VertexArrayDataStruct() {
  glDeleteBuffers(1, &positionBuffer);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteBuffers(1, &texcoordBuffer);
  //glDeleteBuffers(1, &indexBuffer);
  glDeleteVertexArrays(1, &vertexArrayObject);
  //glDeleteTextures(1, &m_texture);
}

void VertexArrayDataStruct::draw() {
  //glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, m_texture);
  glBindVertexArray(vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, size);
//  glDrawArrays(GL_TRIANGLES, 0, size/4);
//  glDrawArrays(GL_TRIANGLES, size/4, size/2);
//  glDrawArrays(GL_TRIANGLES, size/2, size/4);
//  glDrawArrays(GL_TRIANGLES, size-size/4, size/4);
}

}
