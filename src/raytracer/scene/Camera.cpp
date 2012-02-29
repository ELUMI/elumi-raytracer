/*
 * Camera.cpp
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective

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

void Camera::rotate(vec2 rotation){
  //m_direction += vec3(rotation, 0);
  mat4 rot = glm::rotate(mat4(1.0f), -rotation.x, m_up_vector);
  rot = glm::rotate(rot, -rotation.y, cross(m_direction,m_up_vector));
  m_direction = normalize(vec3(rot * vec4(m_direction,0)));
}

void Camera::translate(vec3 translation){
  m_position += mat3(m_direction, cross(m_direction, m_up_vector), m_up_vector) * translation;
}

mat4 Camera::getViewMatrix() const {
  mat4 proj = perspective(float(m_fov*180.0f/M_PI), m_aspect_ratio, 0.1f, 100.f);
  mat4 view = glm::lookAt(m_position, m_position + m_direction, m_up_vector);
  return proj*view;
}

mat4 Camera::getViewportToModelMatrix(float w, float h) const {
  mat4 viewport = transpose(mat4(
      w/2,  0.0f, 0.0f, w/2,
      0.0f, h/2,  0.0f, h/2,
      0.0f, 0.0f, 0.5f, 0.5f,
      0.0f, 0.0f, 0.0f, 1.0f));


  return inverse(viewport * getViewMatrix());
}

}
