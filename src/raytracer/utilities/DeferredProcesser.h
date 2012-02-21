/*
 * DeferredProcesser.h
 *
 *  Created on: Feb 15, 2012
 *      Author: ulvinge
 */

//#include <GL/glew.h>
//#include "../AccDataStructImpl/VertexArrayDataStruct.h"
#include "../scene/Scene.h"

#ifndef DEFERREDPROCESSER_H_
#define DEFERREDPROCESSER_H_

namespace raytracer {
class DeferredProcesser {
public:
  DeferredProcesser(unsigned int width, unsigned int height);
  virtual ~DeferredProcesser();

  void render(Scene* scene, mat4 viewMatrix, int width, int height);
  void readNormals(unsigned int width, unsigned int height, vec3* buffer);
  void readTexCoords(unsigned int width, unsigned int height, vec3* buffer);
  void readDepths(unsigned int width, unsigned int height, float* buffer);

private:
  GLuint shader_program;
  GLuint texcoord_rbo;
  GLuint normal_rbo;
  GLuint depth_rbo;
  GLuint fbo;

};
}

#endif /* DEFERREDPROCESSER_H_ */
