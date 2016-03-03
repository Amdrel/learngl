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
    // The camera has both the view and projection matrices.
    glm::mat4 view;
    glm::mat4 projection;

    // Used to initialize the view matrix.
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 front;
    glm::vec3 up;

    // Used to initialize the projection matrix.
    GLfloat fov;
    GLfloat aspect;
    GLfloat near;
    GLfloat far;

    // Initialize a camera with the defaults (Looking negative z).
    PerspectiveCamera();

    // Create a camera using all params necessary to generate a perspective.
    PerspectiveCamera(const glm::vec3 position, const glm::vec3 rotation,
      const GLfloat fov, const GLfloat aspect, const GLfloat near,
      const GLfloat far);

    // Update the view matrix and the perspective matrix with the current
    // position and front values. In addition, front is also calculated from the
    // rotation vector.
    void update();
};

#endif
