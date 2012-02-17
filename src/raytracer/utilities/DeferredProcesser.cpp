/*
 * DeferredProcesser.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: ulvinge
 */

#include <GL/glew.h>
#include "DeferredProcesser.h"
#include "glutil.h"
#include <glm/gtc/type_ptr.hpp> //value_ptr

namespace raytracer {

DeferredProcesser::DeferredProcesser(unsigned int width, unsigned int height) {
  //************************************
  // The loadShaderProgram and linkShaderProgam functions are defined in glutil.cpp and
  // do exactly what we did in lab1 but are hidden for convenience
  shader_program = loadShaderProgram("deferred.vert", "deferred.frag");
  glBindAttribLocation(shader_program, 0, "position");
  glBindAttribLocation(shader_program, 1, "color");
  glBindAttribLocation(shader_program, 2, "texCoordIn");
  glBindAttribLocation(shader_program, 3, "normal");
  glBindAttribLocation(shader_program, 4, "material");

  glBindFragDataLocation(shader_program, 0, "onormal");
  glBindFragDataLocation(shader_program, 1, "otexcoord");
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
  glGenRenderbuffersEXT(1, &depth_rbo);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rbo);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // create a renderbuffer object to store depth info
  glGenRenderbuffersEXT(1, &normal_rbo);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, normal_rbo);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGB, width, height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // create a renderbuffer object to store depth info
  glGenRenderbuffersEXT(1, &texcoord_rbo);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, texcoord_rbo);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGB, width, height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  CHECK_GL_ERROR();

  // create a framebuffer object
  glGenFramebuffersEXT(1, &fbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  // attach the renderbuffers
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                               GL_RENDERBUFFER_EXT, depth_rbo);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                               GL_RENDERBUFFER_EXT, normal_rbo);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                               GL_RENDERBUFFER_EXT, texcoord_rbo);

  // check FBO status
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
      throw "failed fbo";

  // switch back to window-system-provided framebuffer
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  CHECK_GL_ERROR();
}

DeferredProcesser::~DeferredProcesser() {
  glDeleteFramebuffers(1,&fbo);
  glDeleteRenderbuffersEXT(1,&depth_rbo);
  glDeleteRenderbuffersEXT(1,&normal_rbo);
  glDeleteRenderbuffersEXT(1,&texcoord_rbo);
}

void DeferredProcesser::render(Scene* scene, mat4 viewMatrix){
  // set rendering destination to FBO
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  GLenum buffers_to_render[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2,buffers_to_render);

  void glDrawBuffers( GLsizei n, const GLenum *bufs );

  glViewport(0, 0, 100, 100);
  glPixelZoom(1,1);
  glRasterPos2f(-1,1);
  // clear buffers
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glUseProgram(shader_program);
  int loc = glGetUniformLocation(shader_program, "modelViewProjectionMatrix");
  glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(viewMatrix));

  scene->drawVertexArray();
  glUseProgram(0);
  CHECK_GL_ERROR();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0); //Returns the default FBO to the DRAW location
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0); //Returns the default FBO to the DRAW location

  CHECK_GL_ERROR();
}


void DeferredProcesser::readNormals(unsigned int width, unsigned int height, vec3* buffer){
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
  glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, buffer);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void DeferredProcesser::readTexCoords(unsigned int width, unsigned int height, vec3* buffer){
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);
  glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, buffer);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void DeferredProcesser::readDepths(unsigned int width, unsigned int height, float* buffer){
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  //glReadBuffer(GL_DEPTH_ATTACHMENT_EXT);
  glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, buffer);

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

}
