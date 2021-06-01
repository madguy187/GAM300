#include "pch.h"
#include "Lines3D.h"

Lines3D::Lines3D() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 0 },
    primitiveType{ GL_LINES },
    primitiveCount{ 1 },
    drawCount{ 2 }
{
    initModel();
}

void Lines3D::initModel()
{
    InsertModelData();
    CreateBuffers();
}

void Lines3D::InsertPosVtx()
{
    GLfloat lineSeg[] =
    {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    PosVec.push_back({ lineSeg[0], lineSeg[1], lineSeg[2] });
    PosVec.push_back({ lineSeg[3], lineSeg[4], lineSeg[5] });
}

void Lines3D::InsertIdxVtx()
{

}

void Lines3D::InsertModelData()
{
    InsertPosVtx();
}

GLuint Lines3D::GetVaoID()
{
    return vaoID;
}

GLuint Lines3D::GetVboID()
{
    return vboID;
}

GLuint Lines3D::GetEboID()
{
    return eboID;
}

GLenum Lines3D::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Lines3D::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Lines3D::GetDrawCount()
{
    return drawCount;
}

void Lines3D::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Lines3D::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Lines3D::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Lines3D::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Lines3D::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Lines3D::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Lines3D::CreateVAO()
{
    //Define the VAO handle for position attributes
    glCreateVertexArrays(1, &vaoID);
    glEnableVertexArrayAttrib(vaoID, 0);
    glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(glm::vec3));
    glVertexArrayAttribFormat(vaoID, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 0, 0);

    //Define the VAO handle for texture coordinates
    glEnableVertexArrayAttrib(vaoID, 1);
    glVertexArrayVertexBuffer(vaoID, 1, vboID, sizeof(glm::vec3) * PosVec.size(), sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoID, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 1, 1);

}

void Lines3D::CreateVBO()
{
    glCreateBuffers(1, &vboID);
    glNamedBufferStorage(vboID,
        sizeof(glm::vec3) * PosVec.size() + sizeof(glm::vec2) * TextVec.size(),
        nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vboID, 0,
        sizeof(glm::vec3) * PosVec.size(), PosVec.data());
    glNamedBufferSubData(vboID, sizeof(glm::vec3) * PosVec.size(),
        sizeof(glm::vec2) * TextVec.size(), TextVec.data());
}

void Lines3D::CreateEBO()
{

}

void Lines3D::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Lines3D::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
}
