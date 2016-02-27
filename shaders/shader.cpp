#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <GLFW/glfw3.h>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

}

void Shader::use() {

}

string Shader::readShader(string filepath) {
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

bool Shader::checkCompileStatus(GLuint shader) {
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

bool Shader::checkLinkStatus() {
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(this->program, GL_LINK_STATUS, &success);

  // Panic if there is a shader link error and dump it to stderr.
  if (!success) {
    glGetShaderInfoLog(this->program, 512, nullptr, infoLog);
    cerr << "ERROR: Shader program link failed\n" << infoLog << endl;
    return false;
  }

  return true;
}
