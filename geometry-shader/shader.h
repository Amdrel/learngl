#ifndef SHADER_H
#define SHADER_H

#include <string>

extern "C" {
#include <GL/glew.h>
}

class Shader {
  public:
    // Shader program pointer in the OpenGL state machine.
    GLuint program;

    // Shader constructors, one for vertex and fragment shaders; and one for
    // vertex, fragment, and geometry shaders.
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath,
        const GLchar* geometryPath);

    // Activate the shader in the OpenGL state machine. This is simply just a
    // wrapper around glUseProgram using the public program field as input.
    void use();
  private:
    // Reads a shader (or any file for that matter) and puts it into a string.
    std::string readShader(std::string filepath);

    // Compiles a shader of the specified type.
    GLuint compile(const GLuint type, const GLchar* src);

    // Used to check if a shader compiled successfully.
    bool checkCompileStatus(GLuint shader);

    // Used to check if a shader linked successfully.
    bool checkLinkStatus();
};

#endif
