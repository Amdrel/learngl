#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

extern "C" {
#include <GLFW/glfw3.h>
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
  // Read shader sources into memory.
  std::string vertexShaderSource = readShader(vertexPath);
  std::string fragmentShaderSource = readShader(fragmentPath);

  // Compile the vertex shader.
  const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, nullptr);
  glCompileShader(vertexShader);

  // Check the vertex shader.
  if (!checkCompileStatus(vertexShader)) {
    glfwTerminate();
    exit(1);
  }

  // Compile the fragment shader.
  const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
  glCompileShader(fragmentShader);

  // Check the fragment shader.
  if (!checkCompileStatus(fragmentShader)) {
    glfwTerminate();
    exit(1);
  }

  // Link the vertex and fragment shaders to create the shader program.
  this->program = glCreateProgram();
  glAttachShader(this->program, vertexShader);
  glAttachShader(this->program, fragmentShader);
  glLinkProgram(this->program);

  // Verify the shaders linked successfully.
  if (!checkLinkStatus()) {
    glfwTerminate();
    exit(1);
  }

  // Clean up the unlinked shaders as they are no longer needed.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() {
  glUseProgram(this->program);
}

std::string Shader::readShader(std::string filepath) {
  std::ifstream vertexShaderFile(filepath);

  if (vertexShaderFile) {
    std::ostringstream buffer;
    buffer << vertexShaderFile.rdbuf();
    vertexShaderFile.close();

    return buffer.str();
  } else {
    // Panic when the shader cannot be found. The assumption is made that there
    // is no good reason to use missing shaders.
    std::cerr << "ERROR: Unable to read vertex shader" << std::endl;
    glfwTerminate();
    exit(1);
  }
}

bool Shader::checkCompileStatus(GLuint shader) {
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  // Panic if there is a shader compilation error and dump it to stderr.
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "ERROR: Shader compilation failed\n" << infoLog << std::endl;
    return false;
  }

  return true;
}

bool Shader::checkLinkStatus() {
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(this->program, GL_LINK_STATUS, &success);

  // Panic if there is a shader link error and dump it to stderr.
  if (!success) {
    glGetShaderInfoLog(this->program, 512, nullptr, infoLog);
    std::cerr << "ERROR: Shader program link failed\n" << infoLog << std::endl;
    return false;
  }

  return true;
}
