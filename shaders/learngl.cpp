#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

using namespace std;

// Window constants to keep viewport size consistent.
const GLuint kWindowWidth = 800;
const GLuint kWindowHeight = 600;

// Callbacks
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main() {
  // Initialize GLFW and set some hints that will create an OpenGL 3.3 context
  // using core profile.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a fixed 800x600 window that is not resizable.
  GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight, "LearnGL", nullptr, nullptr);
  if (window == nullptr) {
    cerr << "Failed to created GLFW window" << endl;
    glfwTerminate();
    return 1;
  }

  // Create an OpenGL context and pass callbacks to GLFW.
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, keyCallback);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    glfwTerminate();
    return 1;
  }

  // Create a viewport the same size as the window.
  glViewport(0, 0, kWindowWidth, kWindowHeight);

  // Read and compile the vertex and fragment shaders using
  // the shader helper class.
  Shader shader("glsl/vertex.glsl", "glsl/fragment.glsl");

  // Square vertex data.
  GLfloat vertices[] = {
    // Positions         // Colors
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Top right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // Bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // Top left
  };

  // Indices for the square.
  GLuint indices[] = {
    0, 1, 3, // First triangle
    1, 2, 3  // Second triangle
  };

  // Create a VBO to store the vertex data, an EBO to store indice data, and
  // create a VAO to retain our vertex attribute pointers.
  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind the VAO for the square first, set the vertex buffers,
  // then the attribute pointers.
  glBindVertexArray(VAO);

  // Fill up the VBO and EBO for the square while the VAO for the square is
  // currently bound (see above).
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set the vertex attributes. This set of vertex attributes starts with the
  // vertices and the colors after each vertex with a stride of 6 floats.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // Unbind the VBO and VAO to get a clean state. DO NOT unbind the EBO or the
  // VAO will no longer be able to access it (I have no idea why).
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Render loop.
  while (!glfwWindowShouldClose(window)) {
    // Check and call events.
    glfwPollEvents();

    // Clear the screen to a nice blue color.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate the shader program for this square.
    shader.use();

    // Use sin with time to get a constantly changing color and pass it as a
    // uniform value to the fragment shader.
    GLfloat timeValue = glfwGetTime();
    GLfloat colorValue = (sin(timeValue) / 2) + 0.5;
    GLint vertexColorLocation = glGetUniformLocation(shader.program, "timeColor");
    glUniform4f(vertexColorLocation, colorValue, colorValue, colorValue, 1.0f);

    // Draw the square!
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap buffers used for double buffering.
    glfwSwapBuffers(window);
  }

  // Properly deallocate the VBO and VAO.
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // Terminate GLFW and clean any resources before exiting.
  glfwTerminate();

  return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // Close the application when escape is pressed.
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}
