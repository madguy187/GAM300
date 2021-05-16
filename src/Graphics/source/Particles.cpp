#include "pch.h"
#include "../include/Particles.h"

//const int maxParticles = 10000;

Particles::Particles()
{
    CreateBuffers();
}

void Particles::InsertPosVtx()
{
    Parser input;
    input.ParseFile("meshes/square.json");

    const rapidjson::Value& vertex = input.doc["vertex"].GetArray();

    for (rapidjson::SizeType i = 0; i < vertex.Size(); i++)
    {
        const rapidjson::Value& coords = vertex[i];

        PosVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
            coords[rapidjson::SizeType(1)].GetDouble()));
    }
}

void Particles::InsertTextCoord()
{
    Parser input;
    input.ParseFile("meshes/square.json");

    const rapidjson::Value& texture = input.doc["texture"].GetArray();

    for (rapidjson::SizeType i = 0; i < texture.Size(); i++)
    {
        const rapidjson::Value& coords = texture[i];

        TextVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
            coords[rapidjson::SizeType(1)].GetDouble()));
    }
}

GLuint Particles::GetVaoID()
{
    return vaoID;
}

GLuint Particles::GetVboID()
{
    return vboID;
}

GLuint Particles::GetIndexID()
{
    return indexID;
}

GLuint Particles::GetIndexVecSize()
{
    return static_cast<GLuint>(IndexVec.size());
}

void Particles::GenerateIndices(unsigned int numQuads)
{
    unsigned int index1 = 0;
    unsigned int index2 = 1;
    unsigned int index3 = 2;
    unsigned int index4 = 2;
    unsigned int index5 = 3;
    unsigned int index6 = 0;

    for (unsigned int i = 0; i < numQuads; ++i)
    {
        IndexVec.push_back(index1);
        IndexVec.push_back(index2);
        IndexVec.push_back(index3);
        IndexVec.push_back(index4);
        IndexVec.push_back(index5);
        IndexVec.push_back(index6);

        index1 += 4;
        index2 += 4;
        index3 += 4;
        index4 += 4;
        index5 += 4;
        index6 += 4;
    }
}

void Particles::CreateBuffers()
{
    struct Vertex
    {
        glm::vec2 Position;
        glm::vec4 Color;
        glm::vec2 UV;
    };

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxParticles, nullptr, GL_DYNAMIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV));

    GenerateIndices(maxParticles);

    glCreateBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * IndexVec.size(), IndexVec.data(), GL_STATIC_DRAW);
}

