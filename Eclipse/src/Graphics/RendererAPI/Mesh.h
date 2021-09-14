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
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
        bool NoTex;
        std::vector<Texture> Textures;
        aiColor4D Diffuse;
        aiColor4D Specular;
        aiColor4D Ambient;
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::string MeshName;

        void Setup();
    public:
        Mesh(std::vector<Texture> textures = {});
        Mesh(aiColor4D diff, aiColor4D spec);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein , std::vector<Texture> textures = {});
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular, aiColor4D ambient, bool in, std::string namein);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D spec, bool in);
        void SetMaterialDiffuse(float x, float y, float z);
        void SetMaterialSpecular(float x, float y, float z);
        void Render(Shader& shader, GLenum mode , unsigned int id,unsigned int meshindex);
        void Cleanup();
        void GetAllTextures();

        std::vector<Vertex>& GetVertices();
        unsigned int GetVBOID();
    };
}
#endif