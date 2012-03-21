/*
 * XML.h
 *
 *  Created on: Mar 19, 2012
 *      Author: david
 */

#ifndef XML_H_
#define XML_H_

#include "pugixml.hpp"
#include "../IXML.h"
#include "../scene/Camera.h"
#include "../Settings.h"
#include "../scene/ILight.h"
#include "../scene/LightImpl/OmniLight.h"
#include <glm/glm.hpp>
#include <iostream>
#include <string>

namespace raytracer {

class XML : public IXML {
public:
  XML();

  Scene importScene(const char* fileName);
  void exportScene(Scene scene, const char* fileName);
};

}

#endif /* XML_H_ */
