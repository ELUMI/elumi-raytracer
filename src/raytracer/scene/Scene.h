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
//#include "../utilities/Triangle.h" // AccData inkluderar förmodligen denna
#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "ILight.h"
#include "../Settings.h"

namespace raytracer {

class Scene {
public:
	Scene(Settings* settings);
	virtual ~Scene();

	void loadTriangles(vector<Triangle*> triangles,AABB aabb, bool overwrite=false);
	void loadCamera(Camera camera);
	void loadLights(ILight** lights, size_t length, bool overwrite=false);
	void loadMaterials(Material* materials, size_t length);
	void loadMaterials(std::vector<raytracer::Material*> materials);
	void loadTextures(std::vector<raytracer::Texture*> textures);
	void setSettings(Settings* settings);

	Camera& getCamera();
	IAccDataStruct* getAccDataStruct();

	Settings* getSettings();
	std::vector<ILight*>* getLightVector();
	const std::vector<Material*>& getMaterialVector();
	const std::vector<Texture*>& getTextures();

	Texture* getTextureAt(int index);

	void drawVertexArray();

	IDraw* getDrawable();

private:
	Camera m_camera;
  std::vector<Triangle*> m_triangles;
	IAccDataStruct* m_acc_data_struct;
	std::vector<ILight*>* m_lights;
	std::vector<Material*> m_materials;
  std::vector<Texture*> m_textures;
	IDraw* m_drawable;
	Settings* m_settings;
};

}

#endif /* SCENE_H_ */
