/*
 * IExporter.h
 *
 *  Created on: 20 jan 2012
 *      Author: irri
 */

#ifndef IEXPORTER_H_
#define IEXPORTER_H_

namespace raytracer {

class IExporter {
public:
  virtual ~IExporter() = 0;
	virtual int exportImage(const char* filename, int width, int height, float* buffer)= 0;

};

}

#endif /* IEXPORTER_H_ */
