/*
 * PNGExporter.h
 *
 *  Created on: 20 jan 2012
 *      Author: irri
 */

#ifndef PNGEXPORTER_H_
#define PNGEXPORTER_H_

#include "../IExporter.h"

namespace raytracer {

class PNGExporter : public IExporter {
public:
	PNGExporter();
	virtual ~PNGExporter();

	int exportImage(const char* filename, int width, int height, float* buffer);
};

}

#endif /* PNGEXPORTER_H_ */
