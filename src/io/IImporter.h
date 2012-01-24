/*
 * IImporter.h
 *
 *  Created on: 24 jan 2012
 *      Author: mille
 */

#ifndef IIMPORTER_H_
#define IIMPORTER_H_


namespace raytracer {

class IImporter {
public:

	virtual IImporter(const char* filename);
	virtual ~IImporter();

	virtual int getVertexList() = 0;
	virtual int getNormalList() = 0;
	virtual int getTextureList() = 0;

	virtual int getFaceList() = 0;
	virtual int getMaterialList() = 0;

	virtual int getLightPointList() = 0;
	virtual int getLightQuadList() = 0;
	virtual int getLightDiscList() = 0;


};

}

#endif /* IEXPORTER_H_ */
