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
        unsigned int ID = 0;
        ModelType type = ModelType::UNASSIGNED;
        std::string NameOfModel;
        std::string Directory;
        std::vector<Mesh> Meshes;
        std::vector<Texture> Textures_loaded;
        std::vector<glm::vec3> AllVertices;

        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);
        void ProcessNode(aiNode* node, const aiScene* scene);
        float GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        glm::vec3 ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);

    public:
        bool noTex = false;
        AABB AABB_Property; // For Dynamic AABB

        AssimpModel() { }
        AssimpModel(bool noTex = false);

        void LoadAssimpModel(std::string path);
        void Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID);
        void Cleanup();
        void SetName(std::string& name);
        std::string GetName();
        std::string GetDirectory();
        unsigned int GetNumberOfTextures();
        void GetTextureNames();
        unsigned int GetNumberOfMeshes();
        ModelType GetType();
        void SetModelType(ModelType in);
        std::vector<glm::vec3> GetVertices();
        void SetProperties(std::string& ModelName , ModelType in , unsigned int ID);
        AABB getAABB() const override;
        AABB SetAABB(TransformComponent& in);
    };

}
#endif // ASSIMP_MODEL_CLASS#ifndef ASSIMP_MODEL_CLASS_H