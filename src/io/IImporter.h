/*
 * IImporter.h
 *
 *  Created on: 24 jan 2012
 *      Author: mille
 */

#ifndef IIMPORTER_H_
#define IIMPORTER_H_

#include "../raytracer/utilities/Triangle.h"
#include "../raytracer/scene/Camera.h"
#include "../raytracer/scene/Material.h"

namespace raytracer {

class IImporter {
public:

	IImporter(){;};
	virtual ~IImporter() = 0;

	virtual void loadFile(char* filename) = 0;

	virtual Triangle* gerTriangleList() =0;
	virtual int getTriangleCount() =0;

	virtual Material* getMaterialList() = 0;
	virtual int getMaterialCount() =0;

	virtual Camera getCamera() = 0;

	virtual int getLightPointList() = 0;
	virtual int getLightQuadList() = 0;
	virtual int getLightDiscList() = 0;


};

}

#endif /* IEXPORTER_H_ */
