#ifndef ASSIMP_MODEL_CLASS_H
#define ASSIMP_MODEL_CLASS_H

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
#include <vector>
#include <algorithm>
#include "mesh.h"

// fbx
#include "cimport.h"
#include "Graphics/Grid/IAABB.h"
#include "Graphics/Grid/AABB.h"

namespace Eclipse
{
    enum class ModelType
    {
        UNASSIGNED = 0,
        HUMAN = 1,
        ANIMAL = 2,
        HOUSE = 3,
        ENVIRONMENT = 4,
        MAXCOUNT
    };

    class AssimpModel : public IAABB
    {
    private:
        std::vector<glm::vec3> AllVertices;
        std::string NameOfModel;
        std::vector<Mesh> Meshes;
        std::string Directory;
        std::vector<Texture> Textures_loaded;
        ModelType type = ModelType::UNASSIGNED;

        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);
        void ProcessNode(aiNode* node, const aiScene* scene);

    public:
        bool noTex = false;
        unsigned int ID = 0;
        GLenum GlobalMode = GL_FILL;

        AssimpModel() { }
        AssimpModel(bool noTex = false);

        void LoadAssimpModel(std::string path);
        void Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID);
        void Cleanup();
        void SetName(std::string name);
        std::string GetName();
        std::string GetDirectory();
        unsigned int GetNumberOfTextures();
        void GetTextureNames();
        unsigned int GetNumberOfMeshes();
        ModelType GetType();
        void SetModelType(ModelType in);
        std::vector<glm::vec3> GetVertices();

        AABB test;
        AABB getAABB() const override { return test; };
        void setset(float in) { test.maxX = in; };
    };

}
#endif // ASSIMP_MODEL_CLASS#ifndef ASSIMP_MODEL_CLASS_H