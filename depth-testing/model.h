#include <string>
#include <vector>

#include <assimp/scene.h>

#include "mesh.h"
#include "shader.h"

class Model {
  public:
    Model(std::string path);

    // Draws all the meshes.
    void draw(Shader shader);
  private:
    // Model data.
    std::vector<Mesh> meshes;
    std::string directory;

    // Texture data to prevent duplicate textures.
    std::vector<Texture> loaded_textures;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material,
        aiTextureType type, std::string typeName);
    GLint loadTexture(const char* path, std::string directory);
};
