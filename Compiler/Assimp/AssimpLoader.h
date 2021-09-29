#pragma once
#include "BinarySettings.h"

namespace EclipseCompiler
{
    class MeshA
    {
    public:
        glm::vec4 Diffuse{ 0,0,0,0 };
        glm::vec4 Specular{ 0,0,0,0 };
        glm::vec4 Ambient{ 0,0,0,0 };
        std::array<char, 108> MeshName;
        bool noTex{ true };
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;

        MeshA()
        {
            strcpy_s(MeshName.data(), MeshName.size(), "Mesh");
            MeshName[MeshName.size()-1] = '\0';
        };

        ~MeshA() { };
    };

    struct MeshGeometry
    {
    public:
        glm::vec4 Diffuse{ 0,0,0,0 };
        glm::vec4 Specular{ 0,0,0,0 };
        glm::vec4 Ambient{ 0,0,0,0 };
        std::array<char, 10> MeshName;
        bool NoTex{ true };
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;
        unsigned int VBO = 0;
        unsigned int VAO = 0;
        unsigned int EBO = 0;

        MeshGeometry()
        {
            strcpy_s(MeshName.data(), MeshName.size(), "Mesh");
            MeshName[MeshName.size() - 1] = '\0';
        };

        MeshGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures)
            :
            Vertices(vertices),
            Indices(indices),
            Textures(textures),
            NoTex(false)
        {
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size() - 1] = '\0';
        }

        MeshGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec4  diffuse, glm::vec4  specular, glm::vec4  ambient, bool in, std::string namein)
            :
            Vertices(vertices),
            Indices(indices),
            Diffuse(diffuse),
            Specular(specular),
            Ambient(ambient),
            NoTex(in)
        {
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size() - 1] = '\0';
        }

        ~MeshGeometry() {};
    };

    struct Mesh
    {
    public:
        glm::vec4 Diffuse{ 0,0,0,0 };
        glm::vec4 Specular{ 0,0,0,0 };
        glm::vec4 Ambient{ 0,0,0,0 };
        std::array<char, 10> MeshName;
        bool NoTex{ true };
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;
        unsigned int VBO = 0;
        unsigned int VAO = 0;
        unsigned int EBO = 0;

        Mesh()
        {
            strcpy_s(MeshName.data(), MeshName.size(), "Mesh");
            MeshName[MeshName.size()-1] = '\0';
        };

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures)
            :
            Vertices(vertices),
            Indices(indices),
            Textures(textures),
            NoTex(false)
        {
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size()-1] = '\0';
        }

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec4  diffuse, glm::vec4  specular, glm::vec4  ambient, bool in, std::string namein)
            :
            Vertices(vertices),
            Indices(indices),
            Diffuse(diffuse),
            Specular(specular),
            Ambient(ambient),
            NoTex(in)
        {
            strcpy_s(MeshName.data(), MeshName.size(), namein.data());
            MeshName[MeshName.size()-1] = '\0';
        }

        ~Mesh() {};
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        glm::vec4 Diffuse;
        glm::vec4 Specular;
        glm::vec4 Ambient;
        bool NoTextures = false;
        const char* MeshName;
        std::vector<Texture> textures;
    };

    class AssimpLoader
    {
    public:
        unsigned int MeshIndex = 0;
        std::string NameOfModel;
        std::string Directory;
        std::vector<Mesh> Meshes;
        std::vector<glm::vec3> AllVertices;
        std::vector<MeshData> meshData;
        std::vector<Texture> Textures_loaded;

    public:

        void LoadAssimpModel(std::string path, std::unordered_map<std::string, Mesh>& GeometryContainer);
        void ProcessGeometry(aiNode* node, const aiScene* scene, bool isGeometryCompiler = true);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, const char* MeshName);
        float GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        glm::vec3 ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
        void LoadNewModel(std::unordered_map<std::string, Mesh>& GeometryContainer);

    public:
        std::vector<Texture> ExtractTextures(aiMesh* mesh, const aiScene* scene, std::string& MeshName, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>&);
        void LoadAssimpModelForTextures(std::string path, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>&);
        void ProcessTextures(aiNode* node, const aiScene* scene, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>& TextureContainer);
        std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, const char* MeshName);
        std::vector<Texture> LoadTexturesForCompiler(aiMaterial* mat, aiTextureType type, std::string& MeshName, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>&);
    };
}