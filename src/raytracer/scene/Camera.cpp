/*
 * Camera.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Camera.h"

namespace raytracer {

Camera::Camera() {
	// Setting default camera values
  m_position  = glm::vec3(0.0f, 0.0f, 0.0f);
  m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
  m_up_vector = glm::vec3(0.0f, 1.0f, 0.0f);

  m_fov = 0.7845f;
  m_aspect_ratio = 4.0f/3.0f;
}

Camera::~Camera() {

}

void Camera::set(glm::vec3 position, glm::vec3 direction, glm::vec3 normal, float fov, float aspect_ratio) {
  m_position  = position;
  m_direction = direction;
  m_up_vector    = glm::normalize(normal);

  m_fov = fov;
  m_aspect_ratio = aspect_ratio;
}

void Camera::setPosition(vec3 position) {
  Camera::m_position = position;
}

void Camera::setDirection(vec3 direction) {
  Camera::m_direction = direction;
}

void Camera::setUpVector(vec3 up_vector) {
  Camera::m_up_vector = up_vector;
}

const glm::vec3& Camera::getPosition() {
  return m_position;
}

const glm::vec3& Camera::getDirection() {
  return m_direction;
}

const glm::vec3& Camera::getUpVector() {
  return m_up_vector;
}

const float& Camera::getFov() {
  return m_fov;
}

const float& Camera::getAspectRatio() {
  return m_aspect_ratio;
}

}
