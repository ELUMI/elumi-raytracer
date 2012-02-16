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
    glBindFragDataLocation(shader_program, 0, "fragmentColor");
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

    // create a texture object
    glGenTextures(1, &color_tex);
    glBindTexture(GL_TEXTURE_2D, color_tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // create a texture object
    glGenTextures(1, &normal_tex);
    glBindTexture(GL_TEXTURE_2D, normal_tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


    // create a renderbuffer object to store depth info
    glGenRenderbuffersEXT(1, &depth_rbo);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rbo);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    // create a framebuffer object
    glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, color_tex, 0);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                              GL_TEXTURE_2D, normal_tex, 0);

    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                 GL_RENDERBUFFER_EXT, depth_rbo);

    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
        throw "failed fbo";

    // switch back to window-system-provided framebuffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    CHECK_GL_ERROR();
}

DeferredProcesser::~DeferredProcesser() {
}

void DeferredProcesser::render(Scene* scene, mat4 viewMatrix){
  glUseProgram(shader_program);
  int loc = glGetUniformLocation(shader_program, "modelViewProjectionMatrix");
  glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(viewMatrix));

  scene->drawVertexArray();
  glUseProgram(0);
  CHECK_GL_ERROR();

}

}
