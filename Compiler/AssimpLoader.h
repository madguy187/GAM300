#pragma once

// Assimp
#include "ASSIMP/include/assimp/Importer.hpp"
#include "ASSIMP/include/assimp/scene.h"
#include "ASSIMP/include/assimp/postprocess.h"
#include "ASSIMP/include/assimp/mesh.h"

namespace EclipseCompiler
{
    class Texture
    {
    public:
        Texture(std::string dir, std::string path, aiTextureType type) :
            TextureDirectory(dir),
            TexturePath(path),
            Type(type)
        {

        }

        aiTextureType Type;
        std::string TexturePath;
        std::string TextureDirectory;
    };

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TextureCoodinates;
        aiColor4D m_Color;
    };

    class Mesh
    {
    public:
        std::string MeshName;
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
        bool NoTex;
        aiColor4D Diffuse;
        aiColor4D Specular;
        aiColor4D Ambient;
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures)
            :
            Vertices(vertices),
            Indices(indices),
            Textures(textures),
            NoTex(false),
            MeshName(namein)
        {

        }

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular, aiColor4D ambient, bool in, std::string namein)
            :
            Vertices(vertices),
            Indices(indices),
            Diffuse(diffuse),
            Specular(specular),
            Ambient(ambient),
            NoTex(in),
            MeshName(namein)
        {

        }
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        aiColor4D Diffuse;
        aiColor4D Specular;
        aiColor4D Ambient;
        bool NoTextures = false;
        std::string MeshName;
        std::vector<Texture> textures;
    };

    class AssimpLoader
    {
    private:
        unsigned int MeshIndex = 0;
        unsigned int ID = 0;
        std::string NameOfModel;
        std::string Directory;
        std::vector<Mesh> Meshes;
        std::vector<glm::vec3> AllVertices;
        std::vector<MeshData> meshData;
        std::vector<Texture> Textures_loaded;

    public:
        std::unordered_map<std::string, std::string> ModelMap;

        void LoadModels(const std::string& modelFile);
        void LoadAssimpModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string& MeshName);
        std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, std::string& MeshName);
        float GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        glm::vec3 ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void LoadNewModel();
    };
}