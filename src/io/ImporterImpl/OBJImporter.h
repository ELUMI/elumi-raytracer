/*
 * PNGExporter.h
 *
 *  Created on: 24 jan 2012
 *      Author: mille
 */

#ifndef OBJIMPORTER_H_
#define OBJIMPORTER_H_

#include "../IImporter.h"
#include <obj_loader.h>

namespace raytracer {

class OBJImporter : public IImporter{
public:
		OBJImporter(){;};
		virtual ~OBJImporter() = 0;

		void loadFile(char* filename);
		Triangle** gerTriangleList();
		int getTriangleCount();

		Material** getMaterialList();
		int getMaterialCount();

		Camera* getCamera();

		int getLightPointList();
		int getLightQuadList();
		int getLightDiscList();
private:
		Triangle** triangles;
		int triangle_count;

		Material** materials;
		int material_count;

		Camera* camera;
};

}

#endif
