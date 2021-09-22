#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "color4.h"
#include "color4.inl"

namespace Eclipse
{
    class Vertex
    {
    public:
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TextureCoodinates;
        aiColor4D m_Color;
        static std::vector<Vertex> GenList(float* vertices, int noVertices);
    };

    class Mesh
    {
    private:
        void Setup();
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
        unsigned int ID = 0; // Entity ID 

        Mesh(std::vector<Texture> textures = {});
        Mesh(aiColor4D diff, aiColor4D spec);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular, aiColor4D ambient, bool in, std::string namein);
        void SetMaterialDiffuse(float x, float y, float z);
        void SetMaterialSpecular(float x, float y, float z);
        void Render(Shader& shader, GLenum mode, unsigned int id, unsigned int meshindex);
        void Cleanup();
        void GetAllTextures();
        unsigned int GetMeshID();
        void SetID(unsigned int MeshId);
        std::string GetMeshName();

        std::vector<Vertex>& GetVertices();
        unsigned int GetVBOID();
    };
}
#endif