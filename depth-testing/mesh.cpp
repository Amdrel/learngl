#include <iostream>

#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
  std::vector<Texture> textures) {

  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setup();
}

void Mesh::setup() {
  // Generate the buffers needed for the vertices and indices, and the vertex
  // array object for defining how data should be passed to the shader.
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // Fill the VBO with the vertex data.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
    GL_STATIC_DRAW);

  // Fill the EBO with the indice data.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
    &indices[0], GL_STATIC_DRAW);

  // Vertex position data pointer.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  // Vertex normal data pointer.
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (GLvoid*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  // Vertex uv data pointer.
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    (GLvoid*)offsetof(Vertex, uv));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Mesh::draw(Shader shader) {
  // Index counters for the different texture types.
  GLuint diffuseIndex = 0;
  GLuint specularIndex = 0;
  GLuint emissionIndex = 0;

  for (GLuint i = 0; i < textures.size(); i++) {
    std::string index;
    std::string name = textures[i].type;

    // Get the string representation of the current texture's index depending on
    // the type of texture.
    if (name == "texture_diffuse") {
      index = std::to_string(++diffuseIndex);
    } else if (name == "texture_specular") {
      index = std::to_string(++specularIndex);
    } else if (name == "texture_emission") {
      index += std::to_string(++emissionIndex);
    } else {
      continue;
    }

    // Set the material value for the given type to the correct index.
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
    glUniform1f(glGetUniformLocation(shader.program,
      ("material." + name + index).c_str()), i);
  }

  // Bind back to the default texture unit.
  glActiveTexture(GL_TEXTURE0);

  // Draw the mesh in it's glory.
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
