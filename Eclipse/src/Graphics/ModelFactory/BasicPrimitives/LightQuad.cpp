#include "pch.h"
#include "LightQuad.h"

LightQuad::LightQuad() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
    primitiveType{ GL_TRIANGLES },
    primitiveCount{ 2 },
    drawCount{ 6 }
{
    initModel();
}

void LightQuad::initModel()
{
    InsertModelData();
    CreateBuffers();
}

void LightQuad::InsertPosVtx()
{
    Parser input;
    input.ParseFile("meshes/lightsquare.json");

    const rapidjson::Value& vertex = input.doc["vertex"].GetArray();

    for (rapidjson::SizeType i = 0; i < vertex.Size(); i++)
    {
        const rapidjson::Value& coords = vertex[i];

        PosVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
            coords[rapidjson::SizeType(1)].GetDouble()));
    }
}

void LightQuad::InsertIdxVtx()
{
    Parser input;
    input.ParseFile("meshes/lightsquare.json");

    const rapidjson::Value& index = input.doc["index"].GetArray();

    for (rapidjson::SizeType i = 0; i < index.Size(); i++)
    {
        const rapidjson::Value& coords = index[i];

        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(0)].GetUint()));
        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(1)].GetUint()));
        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(2)].GetUint()));
    }
}

void LightQuad::InsertTextCoord()
{
    Parser input;
    input.ParseFile("meshes/lightsquare.json");

    const rapidjson::Value& texture = input.doc["texture"].GetArray();

    for (rapidjson::SizeType i = 0; i < texture.Size(); i++)
    {
        const rapidjson::Value& coords = texture[i];

        TextVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
            coords[rapidjson::SizeType(1)].GetDouble()));
    }
}

void LightQuad::InsertModelData()
{
    InsertIdxVtx();
    InsertTextCoord();
    InsertPosVtx();
}

GLuint LightQuad::GetVaoID()
{
    return vaoID;
}

GLuint LightQuad::GetVboID()
{
    return vboID;
}

GLuint LightQuad::GetEboID()
{
    return eboID;
}

GLenum LightQuad::GetPrimitiveType()
{
    return primitiveType;
}

GLuint LightQuad::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint LightQuad::GetDrawCount()
{
    return drawCount;
}

void LightQuad::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void LightQuad::SetVboID(GLuint id)
{
    this->vboID = id;
}

void LightQuad::SetEboID(GLuint id)
{
    this->eboID = id;
}

void LightQuad::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void LightQuad::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void LightQuad::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void LightQuad::CreateVAO()
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

void LightQuad::CreateVBO()
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

void LightQuad::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
        reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void LightQuad::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void LightQuad::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
}
