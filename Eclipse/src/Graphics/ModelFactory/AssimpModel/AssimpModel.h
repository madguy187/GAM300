#ifndef ASSIMP_MODEL_CLASS_H
#define ASSIMP_MODEL_CLASS_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "mesh.h"

using namespace Eclipse;

class AssimpModel
{
public:
    glm::vec3 pos;
    glm::vec3 size;

    AssimpModel() { }
    AssimpModel(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

    void loadAssimpModel(std::string path);

    void render(Shader shader);

    void cleanup();

public:
    bool noTex;

    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture> textures_loaded;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type);
};
#endif // ASSIMP_MODEL_CLASS