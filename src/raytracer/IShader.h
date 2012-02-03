/*
 * IShader.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef ISHADER_H_
#define ISHADER_H_

#include "IAccDataStruct.h"
#include <glm/glm.hpp>
using namespace glm;

namespace raytracer {


class IShader {
public:
	virtual ~IShader() {};

	virtual glm::vec4 shade(Ray* ray, IAccDataStruct::IntersectionData intersection_data) = 0;
};

}

#endif /* ISHADER_H_ */
