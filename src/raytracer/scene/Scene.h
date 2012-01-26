/*
 * Scene.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "Material.h"
#include "Camera.h"
#include "ILight.h"

namespace raytracer {

class Scene {
public:
	Scene();
	virtual ~Scene();
};

}

#endif /* SCENE_H_ */
