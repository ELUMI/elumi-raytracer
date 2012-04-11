/*
 * IDeferredProcesser.h
 *
 *  Created on: Apr 10, 2012
 *      Author: ulvinge
 */

#include "scene/Scene.h"

#ifndef IDEFERREDPROCESSER_H_
#define IDEFERREDPROCESSER_H_

namespace raytracer {
class IDeferredProcesser {
public:
  IDeferredProcesser(){}
  virtual ~IDeferredProcesser(){}

  virtual void render(Scene* scene, mat4 viewMatrix, int width, int height)=0;
  virtual void readNormals(unsigned int width, unsigned int height, vec3* buffer)=0;
  virtual void readTexCoords(unsigned int width, unsigned int height, vec3* buffer)=0;
  virtual void readDepths(unsigned int width, unsigned int height, float* buffer)=0;

  virtual GLuint getNormalTex()=0;
  virtual GLuint getDepthTex()=0;
};
}

#endif /* IDEFERREDPROCESSER_H_ */
