/*
 * DeferredProcesser.h
 *
 *  Created on: Apr 10, 2012
 *      Author: ulvinge
 */

#include "../IPhotonMap.h"
#include "../scene/Scene.h"

#ifndef PHOTONPROCESSER_H_
#define PHOTONPROCESSER_H_

namespace raytracer {
class PhotonProcesser {
public:
  PhotonProcesser(unsigned int width, unsigned int height);
  virtual ~PhotonProcesser();

  void render(Scene* scene, mat4 viewMatrix, int width, int height, GLuint normal_tex, GLuint depth_tex);
  void readColors(unsigned int width, unsigned int height, vec3* buffer);
  void readPhotons(vector<Photon>& photons);

private:
  GLuint shader_program;
  GLuint color_tex;
  GLuint normal_tex;
  GLuint depth_tex;
  GLuint fbo;

  size_t size;
  GLuint photonBuffer;
  GLuint vertexArrayObject;
  vector<Photon>* photons;
};
}

#endif /* PHOTONPROCESSER_H_ */
