/*
 * XML.h
 *
 *  Created on: Mar 19, 2012
 *      Author: david
 */

#ifndef IXML_H_
#define IXML_H_

#include "scene/Scene.h"
#include "../io/IImporter.h"
#include "../io/ImporterImpl/OBJImporter.h"

namespace raytracer {

class IXML {
public:
  virtual ~IXML()= 0;
  virtual Scene* importScene(const char* fileName) = 0;
  virtual void exportScene(Scene scene, const char* fileName) = 0;
};
}

#endif /* XML_H_ */
