/*
 * IImporter.h
 *
 *  Created on: 24 jan 2012
 *      Author: mille
 */

#ifndef IIMPORTER_H_
#define IIMPORTER_H_

#include <vector>

#include "../raytracer/utilities/Triangle.h"
#include "../raytracer/scene/Camera.h"
#include "../raytracer/scene/Material.h"
#include "../raytracer/scene/Texture.h"
#include "../raytracer/scene/ILight.h"
#include "../raytracer/common.hpp"

namespace raytracer {

class IImporter {
public:

	virtual void loadFile(const char* filename) = 0;
	virtual std::vector<Triangle*>& getTriangleList() =0;
	virtual std::vector<Material*>& getMaterialList() = 0;
	virtual std::vector<Texture*>& getTextures() = 0;

	virtual AABB* getAABB() = 0;
	virtual Camera* getCamera() = 0;

	virtual std::vector<ILight*>& getLightPointList() = 0;
	virtual std::vector<ILight*>& getLightQuadList() = 0;
	virtual std::vector<ILight*>& getLightDiscList() = 0;


};

}

#endif /* IEXPORTER_H_ */
