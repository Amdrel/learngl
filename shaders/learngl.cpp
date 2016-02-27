#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// Window constants to keep viewport size consistent.
const GLuint kWindowWidth = 800;
const GLuint kWindowHeight = 600;

// Utility functions.
bool checkShader(GLuint);
bool checkShaderProgram(GLuint shaderProgram);
string readShader(string filepath);

// Callbacks
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main() {
  // Initialize GLFW and set some hints that will create an OpenGL 3.3 context
  // using core profile.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

  // Read shader sources into memory.
  string vertexShaderSource = readShader("glsl/vertex.glsl");
  string fragmentShaderSource = readShader("glsl/fragment.glsl");

  // Compile and check the vertex shader.
  const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, nullptr);
  glCompileShader(vertexShader);
  if (!checkShader(vertexShader)) {
    glfwTerminate();
    return 1;
  }

  // Compile and check the fragment shader.
  const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
  glCompileShader(fragmentShader);
  if (!checkShader(fragmentShader)) {
    glfwTerminate();
    return 1;
  }

  // Create and link the shader program for the triangle.
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  if (!checkShaderProgram(shaderProgram)) {
    glfwTerminate();
    return 1;
  }

  // Clean up the unlinked shaders as they are no longer needed.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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

  // Bind the VAO first, set the vertex buffers, then the attribute pointers.
  glBindVertexArray(VAO);

  // Copy the vertices into a buffer that OpenGL can use.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Copy the indices into a buffer that OpenGL can use.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set the position attributes.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  // Set the color attributes.
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // Unbind the VBO and VAO to get a clean state. DO NOT unbind the EBO or the
  // VAO will no longer be able to access it.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Render loop. All it does is render an orange triangle,
  // not too exciting yet.
  while (!glfwWindowShouldClose(window)) {
    // Check and call events.
    glfwPollEvents();

    // Clear the screen to a nice blue color.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate the shader program for this square.
    glUseProgram(shaderProgram);

    // Use sin with time to get a constantly changing color and pass it as a
    // uniform value to the fragment shader.
    //GLfloat timeValue = glfwGetTime();
    //GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
    //GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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

bool checkShader(GLuint shader) {
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  // Panic if there is a shader compilation error and dump it to stderr.
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    cerr << "ERROR: Shader compilation failed\n" << infoLog << endl;
    return false;
  }

  return true;
}

bool checkShaderProgram(GLuint shaderProgram) {
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  // Panic if there is a shader link error and dump it to stderr.
  if (!success) {
    glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
    cerr << "ERROR: Shader program link failed\n" << infoLog << endl;
    return false;
  }

  return true;
}

string readShader(string filepath) {
  ifstream vertexShaderFile(filepath);

  if (vertexShaderFile) {
    ostringstream buffer;
    buffer << vertexShaderFile.rdbuf();
    vertexShaderFile.close();

    return buffer.str();
  } else {
    cerr << "Unable to read vertex shader" << endl;
    glfwTerminate();
    exit(1);
  }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // Close the application when escape is pressed.
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}
