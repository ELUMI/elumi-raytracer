/*
 * DeferredProcesser.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: ulvinge
 */

#include <GL/glew.h>
#include "DeferredTexProcesser.h"
#include "glutil.h"
#include <glm/gtc/type_ptr.hpp> //value_ptr

namespace raytracer {

DeferredTexProcesser::DeferredTexProcesser(unsigned int width, unsigned int height) {
  //************************************
  // The loadShaderProgram and linkShaderProgam functions are defined in glutil.cpp and
  // do exactly what we did in lab1 but are hidden for convenience
  shader_program = loadShaderProgram("data/gl_shaders/deferred.vert", "data/gl_shaders/deferred.frag");
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

  glGenTextures(1, &depth_tex);
  glBindTexture(GL_TEXTURE_2D, depth_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
               width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  vec4 ones(1.0, 1.0, 1.0, 1.0);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &ones.x);

  glGenTextures(1, &normal_tex);
  glBindTexture(GL_TEXTURE_2D, normal_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
               width, height, 0, GL_RGB, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &ones.x);

  glGenTextures(1, &texcoord_tex);
  glBindTexture(GL_TEXTURE_2D, texcoord_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
               width, height, 0, GL_RGB, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &ones.x);

  CHECK_GL_ERROR();

  // create a framebuffer object
  glGenFramebuffersEXT(1, &fbo);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  // attach the texture
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, normal_tex, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, texcoord_tex, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // check FBO status
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
      throw "failed fbo";

  // switch back to window-system-provided framebuffer
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  CHECK_GL_ERROR();
}

DeferredTexProcesser::~DeferredTexProcesser() {
  glDeleteFramebuffers(1,&fbo);
  glDeleteTextures(1,&depth_tex);
  glDeleteTextures(1,&normal_tex);
  glDeleteTextures(1,&texcoord_tex);
}

void DeferredTexProcesser::render(Scene* scene, mat4 viewMatrix, int width, int height){
  // set rendering destination to FBO
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

  GLenum buffers_to_render[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2,buffers_to_render);

  void glDrawBuffers( GLsizei n, const GLenum *bufs );

  glViewport(0, 0, width, height);
  glPixelZoom(1,1);
  glRasterPos2f(-1,1);
  // clear buffers
  glClearColor(0,0,-1,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glUseProgram(shader_program);

  scene->getDrawable()->drawWithView(viewMatrix, glGetUniformLocation(shader_program, "modelViewProjectionMatrix"));
  glUseProgram(0);
  CHECK_GL_ERROR();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0); //Returns the default FBO to the DRAW location
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0); //Returns the default FBO to the DRAW location

  CHECK_GL_ERROR();
}


void DeferredTexProcesser::readNormals(unsigned int width, unsigned int height, vec3* buffer){
  glBindTexture(GL_TEXTURE_2D, normal_tex);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, buffer);
  CHECK_GL_ERROR();
}

void DeferredTexProcesser::readTexCoords(unsigned int width, unsigned int height, vec3* buffer){
  glBindTexture(GL_TEXTURE_2D, texcoord_tex);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, buffer);
  CHECK_GL_ERROR();
}

void DeferredTexProcesser::readDepths(unsigned int width, unsigned int height, float* buffer){
  glBindTexture(GL_TEXTURE_2D, depth_tex);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, buffer);
  CHECK_GL_ERROR();
}

}
