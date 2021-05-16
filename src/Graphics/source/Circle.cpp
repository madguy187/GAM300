#include "pch.h"
#include "../src/Graphics/include/Circle.h"

Circle::Circle() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
    primitiveType{ GL_TRIANGLE_FAN }
{
    initModel();
}

void Circle::initModel()
{
    InsertModelData();
    CreateBuffers();

    SetDrawCount(static_cast<unsigned int>(IdxVec.size()));
    SetPrimitiveCount(static_cast<unsigned int>(IdxVec.size()));
}

void Circle::InsertPosVtx()
{

}

void Circle::InsertIdxVtx()
{
}

void Circle::InsertModelData()
{
    InsertIdxVtx();
    InsertPosVtx();
}

GLuint Circle::GetVaoID()
{
    return vaoID;
}

GLuint Circle::GetVboID()
{
    return vboID;
}

GLuint Circle::GetEboID()
{
    return eboID;
}

GLenum Circle::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Circle::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Circle::GetDrawCount()
{
    return drawCount;
}

void Circle::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Circle::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Circle::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Circle::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Circle::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Circle::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Circle::CreateVAO()
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

void Circle::CreateVBO()
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

void Circle::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
        reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void Circle::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Circle::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
}
