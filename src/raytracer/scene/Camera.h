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

	void set(glm::vec3 position, glm::vec3 rotaion, glm::vec3 normal, float fov, float aspect_ratio);

	const glm::vec3& getPosition();
	const glm::vec3& getDirection();
	const glm::vec3& getUpVector();
	const float& getFov();
	const float& getAspectRatio();

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up_vector;

	float m_fov;
	float m_aspect_ratio;

};

}

#endif /* CAMERA_H_ */
