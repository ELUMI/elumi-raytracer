/*
 * DeferredProcesser.h
 *
 *  Created on: Apr 10, 2012
 *      Author: ulvinge
 */

//#include <GL/glew.h>
//#include "../AccDataStructImpl/VertexArrayDataStruct.h"
#include "../scene/Scene.h"

#include "../IDeferredProcesser.h"

#ifndef DEFERREDTEXPROCESSER_H_
#define DEFERREDTEXPROCESSER_H_


namespace raytracer {
class DeferredTexProcesser : public IDeferredProcesser {
public:
  DeferredTexProcesser(unsigned int width, unsigned int height);
  virtual ~DeferredTexProcesser();

  void render(Scene* scene, mat4 viewMatrix, int width, int height);
  void readNormals(unsigned int width, unsigned int height, vec3* buffer);
  void readTexCoords(unsigned int width, unsigned int height, vec3* buffer);
  void readDepths(unsigned int width, unsigned int height, float* buffer);

  GLuint getNormalTex();
  GLuint getDepthTex();

private:
  GLuint shader_program;
  GLuint texcoord_tex;
  GLuint normal_tex;
  GLuint depth_tex;
  GLuint fbo;

};
}

#endif /* DEFERREDTEXPROCESSER_H_ */
