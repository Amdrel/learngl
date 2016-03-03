#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern "C" {
#include <GL/glew.h>
}

class PerspectiveCamera {
  public:
    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 front;
    glm::vec3 up;

    GLfloat fov;
    GLfloat aspect;
    GLfloat near;
    GLfloat far;

    PerspectiveCamera();
    PerspectiveCamera(glm::vec3 position, glm::vec3 rotation, GLfloat fov,
      GLfloat aspect, GLfloat near, GLfloat far);

    // Update the view matrix and the perspective matrix.
    void update();
};

#endif
