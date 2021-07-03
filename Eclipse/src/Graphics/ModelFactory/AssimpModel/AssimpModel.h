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
    bool noTex;

    std::string NameOfModel;
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    glm::vec3 pos;
    glm::vec3 size;

    AssimpModel() { }
    AssimpModel(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

    void LoadAssimpModel(std::string path);
    void Render(Shader shader);
    void Cleanup();
    void ProcessNode(aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);
};
#endif // ASSIMP_MODEL_CLASS