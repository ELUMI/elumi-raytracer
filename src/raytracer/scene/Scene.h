/*
 * Scene.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

//#include "../IAccDataStruct.h"
#include "../utilities/Triangle.h" // AccData inkluderar förmodligen denna
#include "Material.h"
#include "Camera.h"
#include "ILight.h"

namespace raytracer {

class Scene {
public:
	Scene();
	virtual ~Scene();


	void loadTriangles(Triangle* triangles, int length, bool overwrite=false);
	void loadLights(ILight* lights, int length, bool overwrite=false);

private:
	Camera m_camera;
	std::vector<ILight> m_lights;
	std::vector<Material> m_materials;

	IAccDataStructe
};

}

#endif /* SCENE_H_ */
