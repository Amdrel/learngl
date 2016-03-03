#include "perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera() {
  position = glm::vec3(0.0f, 0.0f, 0.0f);
  rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  fov = glm::radians(45.0f);
  aspect = glm::radians(45.0f);
  near = 0.1f;
  far = 100.0f;

  front = glm::vec3(0.0f, 0.0f, -1.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);

  update();
}

PerspectiveCamera::PerspectiveCamera(const glm::vec3 position,
  const glm::vec3 rotation, const GLfloat fov, const GLfloat aspect,
  const GLfloat near, const GLfloat far) {

  this->position = position;
  this->rotation = rotation;
  this->fov = fov;
  this->aspect = aspect;
  this->near = near;
  this->far = far;

  front = glm::vec3(0.0f, 0.0f, -1.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);

  update();
}

void PerspectiveCamera::update() {
  // Update the camera's front vector with the new pitch and yaw values to
  // change where it is pointing.
  front = glm::normalize(glm::vec3(
    cos(rotation.x) * cos(rotation.y),
    sin(rotation.x),
    cos(rotation.x) * sin(rotation.y)
  ));

  view = glm::lookAt(position, position + front, up);
  projection = glm::perspective(fov, aspect, near, far);
}
