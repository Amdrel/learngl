#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

extern "C" {
#include <SOIL/SOIL.h>
}

#include "model.h"

Model::Model(std::string path) {
  loadModel(path);
}

void Model::draw(Shader shader) {
  for (GLuint i = 0; i < meshes.size(); i++) {
    meshes[i].draw(shader);
  }
}

void Model::loadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
      aiProcess_FlipUVs);

  // Verify that the model loaded properly.
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "ERROR: " << importer.GetErrorString() << std::endl;
    delete scene;
    return;
  }

  // Save the directory of the model and go to the next stage in the pipeline.
  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  // Find all meshes in the node and process them.
  for (GLuint i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  // Process all children nodes of this node.
  for (GLuint i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  for (GLuint i = 0; i < mesh->mNumVertices; i++) {
    // Copy the vertex positions and normals into our vertex data structure.
    // This is probably really inefficient.
    Vertex vertex;
    vertex.position = glm::vec3(
      mesh->mVertices[i].x,
      mesh->mVertices[i].y,
      mesh->mVertices[i].z
    );
    vertex.normal = glm::vec3(
      mesh->mNormals[i].x,
      mesh->mNormals[i].y,
      mesh->mNormals[i].z
    );

    // Copy only the first texture coordinate pair. Apparently you can have up
    // to 8 pairs of texture coordinates (maps?).
    if (mesh->mTextureCoords[0]) {
      vertex.uv = glm::vec2(
        mesh->mTextureCoords[0][i].x,
        mesh->mTextureCoords[0][i].y
      );
    } else {
      vertex.uv = glm::vec2(0.0f, 0.0f);
    }

    // Push the vertex onto the mesh.
    vertices.push_back(vertex);
  }

  // Add all the indices in all of the mesh's faces.
  for (GLuint i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    // Add the indices for the current face.
    for (GLuint j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Get the material diffuse and specular values.
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
      aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  std::vector<Texture> specularMaps = loadMaterialTextures(material,
      aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material,
    aiTextureType type, std::string typeName) {
  std::vector<Texture> textures;

  for(GLuint i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;
    material->GetTexture(type, i, &str);

    GLboolean skip = false;
    for (GLuint j = 0; j < loaded_textures.size(); j++) {
      if (loaded_textures[j].path == str) {
        textures.push_back(loaded_textures[j]);
        skip = true;
        break;
      }
    }

    if (!skip) {
      Texture texture;
      texture.id = loadTexture(str.C_Str(), directory);
      texture.type = typeName;
      texture.path = str;
      textures.push_back(texture);
      loaded_textures.push_back(texture);
    }
  }

  return textures;
}

GLint Model::loadTexture(const char* path, std::string directory) {
  // Generate texture ID and load texture data.
  std::string filename(path);
  filename = directory + '/' + filename;
  GLuint textureID;
  glGenTextures(1, &textureID);
  int width, height;
  unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

  // Assign texture to ID.
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Parameters.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);

  return textureID;
}
