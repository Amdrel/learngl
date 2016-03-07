#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/scene.h>

extern "C" {
#include <GL/glew.h>
}

#include "shader.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

struct Texture {
  GLuint id;
  std::string type;
  aiString path;
};

class Mesh {
  public:
    // Generic raw mesh data.
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
      std::vector<Texture> textures);

    // Draw the mesh with the given shader program.
    void draw(Shader shader);
  private:
    // OpenGL state data.
    GLuint VAO, VBO, EBO;

    // Works with OpenGL to create buffers to store the mesh data on the GPU.
    void setup();
};

#endif
