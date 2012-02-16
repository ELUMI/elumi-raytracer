/*
 * Scene.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "../IAccDataStruct.h"
#include "../AccDataStructImpl/VertexArrayDataStruct.h"
//#include "../utilities/Triangle.h" // AccData inkluderar förmodligen denna
#include "Material.h"
#include "Camera.h"
#include "ILight.h"

namespace raytracer {

class Scene {
public:
	Scene();
	virtual ~Scene();

	void loadTriangles(vector<Triangle*> triangles, bool overwrite=false);
	void loadCamera(Camera camera);
	void loadLights(ILight* lights, int length, bool overwrite=false);
	void loadMaterials(Material* materials, int length);
	void loadMaterials(std::vector<raytracer::Material*> materials);


	const Camera& getCamera();
	IAccDataStruct* getAccDataStruct();

	const std::vector<ILight*>& getLightVector();
	const std::vector<Material*>& getMaterialVector();

	void drawVertexArray();

private:
	Camera m_camera;
	IAccDataStruct* m_acc_data_struct;
	std::vector<ILight*> m_lights;
	std::vector<Material*> m_materials;
	VertexArrayDataStruct m_vertex_array;
};

}

#endif /* SCENE_H_ */
