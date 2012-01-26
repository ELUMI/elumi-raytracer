/*
 * IExporter.h
 *
 *  Created on: 20 jan 2012
 *      Author: irri
 */

#ifndef IEXPORTER_H_
#define IEXPORTER_H_

#include <stdint.h>


namespace raytracer {

class IExporter {
public:

	IExporter();
	virtual ~IExporter();
	virtual int exportImage(const char* filename, int width, int height, uint8_t* buffer) = 0;

};

}

#endif /* IEXPORTER_H_ */
