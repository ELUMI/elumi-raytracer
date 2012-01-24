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

class IImporter {
public:

	virtual int importImage(const char* filename) = 0;

};

}

#endif /* IEXPORTER_H_ */
