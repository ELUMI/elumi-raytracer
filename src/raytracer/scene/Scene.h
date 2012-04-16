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
#include "../IEnvironmentMap.h"
#include "../Settings.h"

namespace raytracer {

class Scene {
public:
	Scene(Settings* settings);
	virtual ~Scene();

	void loadTriangles(vector<Triangle*> triangles,AABB aabb, size_t material_shift);
	void loadCamera(Camera camera);
	void loadLights(ILight** lights, size_t length, bool overwrite=false);
	size_t loadMaterials(Material* materials, size_t length);
	size_t loadMaterials(std::vector<raytracer::Material*> materials);
	void loadTextures(std::vector<raytracer::Texture*> textures);
	void setEnvirontmentMap(IEnvironmentMap* environment_map);
	void setSettings(Settings* settings);

	Camera& getCamera();
	IAccDataStruct* getAccDataStruct();

	Settings* getSettings();
	std::vector<ILight*>* getLightVector();
	const std::vector<Material*>& getMaterialVector();
	const std::vector<Texture*>& getTextures();
	IEnvironmentMap* getEnvironmentMap();

	Texture* getTextureAt(int index);

	void drawVertexArray();

	IDraw* getDrawable();
  void build();

private:
	Camera m_camera;
  std::vector<Triangle*> m_triangles;
	IAccDataStruct* m_acc_data_struct;
	std::vector<ILight*>* m_lights;
	std::vector<Material*> m_materials;
  std::vector<Texture*> m_textures;
	IDraw* m_drawable;
	Settings* m_settings;
  IEnvironmentMap* environment_map;
  AABB aabb;
};

}

#endif /* SCENE_H_ */
