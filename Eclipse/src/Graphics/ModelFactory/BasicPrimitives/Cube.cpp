#include "pch.h"
#include "Cube.h"

Cube::Cube() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
    primitiveType{ GL_TRIANGLES },
    primitiveCount{ 2 },
    drawCount{ 6 },
    TriCount{ 0 }
{
    initModel();
}

void Cube::initModel()
{
    InsertModelData();
    CreateBuffers();

}

void Cube::InsertPosVtx()
{
    Plane_ planemesh;

    glm::vec3 const translateArray[] =
    {
        glm::vec3(0.0f,  0.0f,  0.5f), // Z+
        glm::vec3(0.0f,  0.0f, -0.5f), // Z-
        glm::vec3(0.5f,  0.0f,  0.0f), // X+
        glm::vec3(-0.5f,  0.0f,  0.0f), // X-
        glm::vec3(0.0f,  0.5f,  0.0f), // Y+
        glm::vec3(0.0f, -0.5f,  0.0f), // Y-
    };

    glm::vec2 const rotateArray[] =
    {
        glm::vec2(0.0f, 0.0f),             // Z+
        glm::vec2(0.0f, (float)PI),        // Z-
        glm::vec2(0.0f, (float)HALF_PI),   // X+       
        glm::vec2(0.0f, (float)HALF_PI),   // X-
        glm::vec2((float)HALF_PI, 0.0f),   // Y+
        glm::vec2((float)HALF_PI, 0.0f)    // Y-
    };

    /*  Transform the plane to 6 positions to form the faces of the cube */
    for (int i = 0; i < 6; ++i)
    {
        Mat4 transformMat = Translate(translateArray[i]) * Rotate(rotateArray[i][YINDEX], YAXIS) * Rotate(rotateArray[i][XINDEX], XAXIS);

        for (int j = 0; j < 4; ++j)
        {
            glm::vec3 posVtx = glm::vec3(transformMat * glm::vec4(planemesh.PosVec[j], 1.0));
            glm::vec3 nrmVtx = glm::vec3(transformMat * glm::vec4(planemesh.NormalVec[j], 1.0));

            posVtx = RoundDecimal(posVtx);
            nrmVtx = RoundDecimal(nrmVtx);

            PosVec.push_back(posVtx);
            NormalVec.push_back(nrmVtx);
            TextVec.push_back(planemesh.TextVec[j]);
        }
    }
}

void Cube::InsertIdxVtx()
{
    Plane_ planemesh;

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            IdxVec.push_back(planemesh.IdxVec[j] + 4 * i);
        }
    }
}

void Cube::InsertTextCoord()
{
}

void Cube::InsertModelData()
{
    InsertPosVtx();
    InsertIdxVtx();
}

GLuint Cube::GetVaoID()
{
    return vaoID;
}

GLuint Cube::GetVboID()
{
    return vboID;
}

GLuint Cube::GetEboID()
{
    return eboID;
}

GLenum Cube::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Cube::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Cube::GetDrawCount()
{
    //return drawCount;
    return static_cast<GLuint>(IdxVec.size());
}

void Cube::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Cube::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Cube::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Cube::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Cube::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Cube::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Cube::CreateVAO()
{
    //Define the VAO handle for position attributes
    glCreateVertexArrays(1, &vaoID);
    glEnableVertexArrayAttrib(vaoID, 0);
    glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(glm::vec3));
    glVertexArrayAttribFormat(vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 0, 0);

    //Define the VAO handle for texture coordinates
    glEnableVertexArrayAttrib(vaoID, 1);
    glVertexArrayVertexBuffer(vaoID, 1, vboID, sizeof(glm::vec3) * PosVec.size(), sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoID, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoID, 1, 1);

}

void Cube::CreateVBO()
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

void Cube::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(), reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void Cube::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Cube::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
}

bool Cube::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}

GLuint Cube::GetIndicesCount()
{
    return static_cast<GLuint>(IdxVec.size());
}
