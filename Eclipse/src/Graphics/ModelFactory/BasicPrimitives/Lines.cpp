#include "pch.h"
#include "Lines.h"

Lines::Lines() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 0 },
    primitiveType{ GL_LINES },
    primitiveCount{ 1 },
    drawCount{ 2 }
{
    initModel();
}

void Lines::initModel()
{
    InsertModelData();
    CreateBuffers();
}

void Lines::InsertPosVtx()
{
    GLfloat lineSeg[] =
    {
        0.0f, 0.0,
        1.0f, 0.0f
    };

    PosVec.push_back({ lineSeg[0], lineSeg[1] });
    PosVec.push_back({ lineSeg[2], lineSeg[3] });
}

void Lines::InsertIdxVtx()
{
    Parser input;
    input.ParseFile("src/Assets/meshes/square.json");

    const rapidjson::Value& index = input.doc["index"].GetArray();

    for (rapidjson::SizeType i = 0; i < index.Size(); i++)
    {
        const rapidjson::Value& coords = index[i];

        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(0)].GetUint()));
        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(1)].GetUint()));
        IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(2)].GetUint()));
    }
}

void Lines::InsertModelData()
{
    InsertIdxVtx();
    InsertPosVtx();
}

GLuint Lines::GetVaoID()
{
    return vaoID;
}

GLuint Lines::GetVboID()
{
    return vboID;
}

GLuint Lines::GetEboID()
{
    return eboID;
}

GLenum Lines::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Lines::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Lines::GetDrawCount()
{
    return drawCount;
}

void Lines::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Lines::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Lines::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Lines::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Lines::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Lines::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Lines::CreateVAO()
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

void Lines::CreateVBO()
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

void Lines::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * PosVec.size(),
        reinterpret_cast<GLvoid*>(PosVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void Lines::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Lines::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
}
