/*
 * PNGExporter.h
 *
 *  Created on: 24 jan 2012
 *      Author: mille
 */

#ifndef OBJIMPORTER_H_
#define OBJIMPORTER_H_

#include <stdexcept>

#include "../IImporter.h"

namespace raytracer {

class ImageNotLoadedException : public runtime_error {
public:
  ImageNotLoadedException() : runtime_error("The image was not loaded.") {}
};

class OBJImporter : public IImporter{
public:
		OBJImporter();
		virtual ~OBJImporter();

		void loadFile(char* filename);
		std::vector<Triangle*>& getTriangleList();
		std::vector<Material*>& getMaterialList();
		std::vector<Texture*>& getTextures();

		Camera* getCamera();

		std::vector<ILight*>&  getLightPointList();
		std::vector<ILight*>&  getLightQuadList();
		std::vector<ILight*>&  getLightDiscList();
private:
		std::vector<Triangle*> triangles;
		std::vector<Material*> materials;
		std::vector<Texture*> textures;

		Camera* camera;

		std::vector<ILight*> point_lights;
		std::vector<ILight*> quad_lights;
		std::vector<ILight*> disc_lights;
};

}

#endif
