#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "assimp/color4.h"
#include "ASSIMP/color4.inl"

using namespace Eclipse;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoodinates;
    static std::vector<Vertex> GenList(float* vertices, int noVertices);
};

class Mesh
{
public:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    unsigned int VAO;
    std::vector<Texture> Textures;
    aiColor4D Diffuse;
    aiColor4D Specular;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures = {});
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D spec);

    void Render(Shader& shader, GLenum mode);
    void Cleanup();

private:
    unsigned int VBO, EBO;
    bool NoTex;
    void Setup();
};

#endif