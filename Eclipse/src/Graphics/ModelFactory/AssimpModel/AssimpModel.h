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
private:
    std::string NameOfModel;
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);
    void ProcessNode(aiNode* node, const aiScene* scene);

public:
    bool noTex;
    glm::vec3 pos;
    glm::vec3 size;

    AssimpModel() { }
    AssimpModel(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

    void LoadAssimpModel(std::string path);
    void Render(Shader shader);
    void Cleanup();
    void SetName(std::string name);
    std::string GetName();
    std::string GetDirectory();
};
#endif // ASSIMP_MODEL_CLASS