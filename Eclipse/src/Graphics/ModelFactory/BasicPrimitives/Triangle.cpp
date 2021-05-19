#include "pch.h"

Triangle::Triangle() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
    primitiveType{ GL_TRIANGLES },
    primitiveCount{ 1 },
    drawCount{ 3 }
{
    initModel();
}

void Triangle::initModel()
{
    InsertModelData();
    CreateBuffers();
}

void Triangle::InsertPosVtx()
{
    Parser input;
    input.ParseFile("meshes/triangle.json");

    const rapidjson::Value& vertex = input.doc["vertex"].GetArray();

    for (rapidjson::SizeType i = 0; i < vertex.Size(); i++)
    {
        const rapidjson::Value& coords = vertex[i];

        PosVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
            coords[rapidjson::SizeType(1)].GetDouble()));
    }
}

void Triangle::InsertIdxVtx()
{
    Parser input;
    input.ParseFile("meshes/triangle.json");

    const rapidjson::Value& index = input.doc["index"].GetArray();

    for (rapidjson::SizeType i = 0; i < index.Size(); i++)
    {
        const rapidjson::Value& coords = index[i];

        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(0)].GetUint()));
        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(1)].GetUint()));
        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(2)].GetUint()));
    }
}

void Triangle::InsertModelData()
{
    InsertIdxVtx();
    InsertPosVtx();
}

GLuint Triangle::GetVaoID()
{
    return vaoID;
}

GLuint Triangle::GetVboID()
{
    return vboID;
}

GLuint Triangle::GetEboID()
{
    return eboID;
}

GLenum Triangle::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Triangle::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Triangle::GetDrawCount()
{
    return drawCount;
}

void Triangle::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Triangle::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Triangle::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Triangle::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Triangle::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Triangle::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Triangle::CreateVAO()
{
    //Define the VAO handle for position attributes
    glCreateVertexArrays(1, &vaoID);
    glEnableVertexArrayAttrib(vaoID, 0);
    glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoID, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 0, 0);

    //Define the VAO handle for texture coordinates
    glEnableVertexArrayAttrib(vaoID, 1);
    glVertexArrayVertexBuffer(vaoID, 1, vboID, sizeof(glm::vec2) * PosVec.size(), sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoID, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 1, 1);
}

void Triangle::CreateVBO()
{
    glCreateBuffers(1, &vboID);
    glNamedBufferStorage(vboID,
        sizeof(glm::vec2) * PosVec.size() + sizeof(glm::vec2) * TextVec.size(),
        nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vboID, 0,
        sizeof(glm::vec2) * PosVec.size(), PosVec.data());
    glNamedBufferSubData(vboID, sizeof(glm::vec2) * PosVec.size(),
        sizeof(glm::vec2) * TextVec.size(), TextVec.data());
}

void Triangle::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
        reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void Triangle::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Triangle::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
}
