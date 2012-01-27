/*
 * Camera.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

namespace raytracer {

class Camera {
public:
	Camera();
	virtual ~Camera();

private:
	glm::vec4 m_view_matrix;

	//glm::vec3 m_position;
};

}

#endif /* CAMERA_H_ */
