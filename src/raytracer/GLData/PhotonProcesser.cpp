/*
 * DeferredProcesser.cpp
 *
 *  Created on: Apr 10, 2012
 *      Author: ulvinge
 */

#include <GL/glew.h>
#include "PhotonProcesser.h"
#include "glutil.h"
#include <glm/gtc/type_ptr.hpp> //value_ptr

namespace raytracer {

PhotonProcesser::PhotonProcesser(unsigned int width, unsigned int height) {
  //************************************
  // The loadShaderProgram and linkShaderProgam functions are defined in glutil.cpp and
  // do exactly what we did in lab1 but are hidden for convenience
  shader_program = loadShaderProgram("data/gl_shaders/photon.vert", "data/gl_shaders/photon.frag");
  glBindAttribLocation(shader_program, 0, "direction");
  glBindAttribLocation(shader_program, 1, "position");
  glBindAttribLocation(shader_program, 2, "normal");
  glBindAttribLocation(shader_program, 3, "power");
  glBindAttribLocation(shader_program, 4, "normal_tex");
  glBindAttribLocation(shader_program, 5, "depth_tex");

  glBindFragDataLocation(shader_program, 0, "ocolor");
  linkShaderProgram(shader_program);

  //************************************
  //        Set uniforms
  //************************************

  glUseProgram(shader_program);
  CHECK_GL_ERROR();

  // Get the location in the shader for uniform tex0
  //int texLoc = glGetUniformLocation(shader_program, "colortexture");
  // Set colortexture to 0, to associate it with texture unit 0
  //glUniform1i(texLoc, 0);

  // create a renderbuffer object to store depth info
  glGenTextures(1, &color_tex);
  glBindTexture(GL_TEXTURE_2D, color_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
               width, height, 0, GL_RGB, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  vec4 ones(1.0, 1.0, 1.0, 1.0);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &ones.x);

  CHECK_GL_ERROR();

  // create a framebuffer object
  glGenFramebuffersEXT(1, &fbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  // attach the renderbuffers
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // check FBO status
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
      throw "failed fbo";

  // switch back to window-system-provided framebuffer
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  CHECK_GL_ERROR();
  size = 0;
}

PhotonProcesser::~PhotonProcesser() {
  glDeleteFramebuffers(1,&fbo);
  glDeleteTextures(1,&color_tex);
  CHECK_GL_ERROR();
}

void PhotonProcesser::readPhotons(vector<Photon>& photons) {
  this->photons = &photons;
  //make buffers
  glGenBuffers(1, &photonBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, photonBuffer);
  glBufferData(GL_ARRAY_BUFFER, photons.size() * sizeof(Photon),
      photons.data(), GL_STATIC_DRAW);

  CHECK_GL_ERROR();

  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, photonBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, false/*normalized*/, sizeof(Photon)/*stride*/, (const GLvoid*)(0*sizeof(vec3))/*offset*/);
  glVertexAttribPointer(1, 3, GL_FLOAT, false/*normalized*/, sizeof(Photon)/*stride*/, (const GLvoid*)(1*sizeof(vec3))/*offset*/);
  glVertexAttribPointer(2, 3, GL_FLOAT, false/*normalized*/, sizeof(Photon)/*stride*/, (const GLvoid*)(2*sizeof(vec3))/*offset*/);
  glVertexAttribPointer(3, 3, GL_FLOAT, false/*normalized*/, sizeof(Photon)/*stride*/, (const GLvoid*)(3*sizeof(vec3))/*offset*/);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  CHECK_GL_ERROR();
  size = photons.size();
}

void PhotonProcesser::render(Scene* scene, mat4 viewMatrix, int width, int height, GLuint normal_tex, GLuint depth_tex){
  CHECK_GL_ERROR();
  // set rendering destination to FBO
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  GLenum buffers_to_render[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1,buffers_to_render);

  CHECK_GL_ERROR();

  glViewport(0, 0, width, height);
  glPixelZoom(1,1);
  glRasterPos2f(-1,1);
  // clear buffers
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc (GL_ONE, GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glPointSize(10);

  CHECK_GL_ERROR();

  glUseProgram(shader_program);

  setUniformSlow(shader_program, "depth_tex", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depth_tex);
  setUniformSlow(shader_program, "normal_tex", 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, normal_tex);

  glBegin(GL_POINTS);
  for(size_t i=0; i<size;++i){
    Photon p = photons->at(i);
    vec3 c = p.power;
    //c = vec4(0,1,0,0);
    //c = glm::normalize(c);
    vec3 v = p.position;
    glColor3f(c.r, c.b, c.g);
    glVertex3f(v.x, v.y, v.z);
  }
  glEnd();

  glBindVertexArray(vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, size);

  glUseProgram(0);
  glPointSize(1);
  CHECK_GL_ERROR();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0); //Returns the default FBO to the DRAW location
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0); //Returns the default FBO to the DRAW location

  CHECK_GL_ERROR();
}

void PhotonProcesser::readColors(unsigned int width, unsigned int height, vec3* buffer){
  glBindTexture(GL_TEXTURE_2D, color_tex);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, buffer);
  CHECK_GL_ERROR();
}

}
