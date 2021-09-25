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
        glm::vec3 Position{ 0,0,0 };
        glm::vec3 Normal{ 0,0,0 };
        glm::vec2 TextureCoodinates{ 0,0 };
        glm::vec4 m_Color{ 0,0,0,0 };
        static std::vector<Vertex> GenList(float* vertices, int noVertices);
    };

    class Mesh
    {
    private:
        void Setup();
    public:
        unsigned int VBO = 0;
        unsigned int VAO = 0;
        unsigned int EBO = 0;

        std::string MeshName;
        bool NoTex = false;
        glm::vec4 Diffuse{ 0,0,0,0 };
        glm::vec4 Specular{ 0,0,0,0 };
        glm::vec4 Ambient{ 0,0,0,0 };
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;
        unsigned int ID = 0; // Entity ID 

        Mesh(std::vector<Texture> textures = {});
        Mesh(glm::vec4 diff, glm::vec4 spec);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 ambient, bool in, std::string namein);
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