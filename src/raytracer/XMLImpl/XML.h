/*
 * XML.h
 *
 *  Created on: Mar 19, 2012
 *      Author: david
 */

#ifndef XML_H_
#define XML_H_

#include "pugixml/pugixml.hpp"
#include "../IXML.h"
#include <glm/glm.hpp>
#include <iostream>
#include <string>

namespace raytracer {

class XML : public IXML {
public:
  XML(int open_gl_version);
  ~XML();

  Scene* importScene(const char* fileName, const char* settingsFileName);
  void exportScene(Scene scene, const char* fileName);

  private:
  void getSettings(const char* xml_file, Settings* settings);
  int open_gl_version;
};

}

#endif /* XML_H_ */
