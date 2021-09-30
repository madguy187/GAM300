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
    //TEST CODE
    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        glm::vec4 Diffuse;
        glm::vec4 Specular;
        glm::vec4 Ambient;

        bool NoTextures = false;
        std::vector<Texture> textures;
        std::string MeshName;
    };

    class AssimpModel
    {
    private:
        unsigned int MeshIndex = 0;
        unsigned int ID = 0;
        ModelType type = ModelType::MT_UNASSIGNED;
        std::string NameOfModel;
        std::string Directory;
        std::vector<Mesh> Meshes;
        std::vector<Texture> Textures_loaded;
        std::vector<glm::vec3> AllVertices;
        std::vector<MeshData> meshData;

        void LoadNewModel();
        void ProcessNode(aiNode* node, const aiScene* scene);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string& MeshName);
        std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, std::string& MeshName);
        float GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        glm::vec3 ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);

    public:

        AssimpModel() { }
        AssimpModel(bool noTex = false);
        AssimpModel(bool noTex, std::string& NameOfModels, std::string& Directorys, std::vector<Mesh> Meshess, std::vector<Texture> Textures_loadeds);

        void LoadAssimpModel(std::string path);
        void Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID, unsigned int id);
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
        void SetProperties(std::string& ModelName, ModelType in, unsigned int ID);
        void SetProperties(std::string& ModelName, ModelType in);
        unsigned int GetEntityID();
        std::vector<Mesh> GetMesh();
        std::vector<Texture> GetTextures();
    };

}
#endif // ASSIMP_MODEL_CLASS#ifndef ASSIMP_MODEL_CLASS_H