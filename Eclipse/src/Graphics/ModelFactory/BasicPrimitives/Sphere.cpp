#include "pch.h"
#include "Sphere.h"

#define M_PI       3.14159265358979323846   // pi

Sphere::Sphere() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
    primitiveType{ GL_TRIANGLES },
    primitiveCount{ 2 },
    drawCount{ 6 },
    TriCount{ 0 }
{
    initModel();
}

void Sphere::initModel()
{
    InsertModelData();
    CreateBuffers();
}

void Sphere::InsertPosVtx()
{
    float radius = 0.5f;

    for (int stack = 0; stack <= 32; ++stack)
    {
        float row = static_cast<float>(stack) / 32;
        float beta = static_cast<float>(M_PI * (row - 0.5f));

        for (int slice = 0; slice <= 32; ++slice)
        {
            float col = static_cast<float>(slice) / 32;
            float alpha = static_cast<float>(col * M_PI * 2.0f);

            glm::vec3 posVtx{ radius * sin(alpha) * cos(beta), radius * sin(beta), radius * cos(alpha) * cos(beta) };

            PosVec.push_back(posVtx);
            NormalVec.push_back(posVtx / radius);
            TextVec.push_back(glm::vec2{ col, 1 - row });
        }
    }

    //Parser input;
    //input.ParseFile("../meshes/square.json");
    //
    //const rapidjson::Value& vertex = input.doc["vertex"].GetArray();
    //
    //for (rapidjson::SizeType i = 0; i < vertex.Size(); i++)
    //{
    //    const rapidjson::Value& coords = vertex[i];
    //
    //    PosVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
    //        coords[rapidjson::SizeType(1)].GetDouble()));
    //}
}

void Sphere::InsertIdxVtx()
{
    int i0 = 0, i1 = 0, i2 = 0;

    int stride = 32 + 1;

    for (int i = 0; i < 32; ++i)
    {
        int currRow = i * stride;

        for (int j = 0; j < 32; ++j)
        {
            /*  You need to compute the indices for the first triangle here */
            i0 = currRow + j;
            i1 = i0 + 1;
            i2 = i1 + stride;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(PosVec[i0],
                PosVec[i1],
                PosVec[i2]))
            {

                IdxVec.push_back(i0);
                IdxVec.push_back(i1);
                IdxVec.push_back(i2);

                ++TriCount;
            }

            /*  You need to compute the indices for the second triangle here */
            i0 = i2;
            i1 = i0 - 1;
            i2 = currRow + j;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(PosVec[i0],
                PosVec[i1],
                PosVec[i2]))
            {
                IdxVec.push_back(i0);
                IdxVec.push_back(i1);
                IdxVec.push_back(i2);

                ++TriCount;
            }
        }
    }

    //Parser input;
    //input.ParseFile("../meshes/square.json");
    //
    //const rapidjson::Value& index = input.doc["index"].GetArray();
    //
    //for (rapidjson::SizeType i = 0; i < index.Size(); i++)
    //{
    //    const rapidjson::Value& coords = index[i];
    //
    //    IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(0)].GetUint()));
    //    IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(1)].GetUint()));
    //    IdxVec.push_back(static_cast<GLushort>(coords[rapidjson::SizeType(2)].GetUint()));
    //}
}

void Sphere::InsertTextCoord()
{
    //Parser input;
    //input.ParseFile("../meshes/square.json");
    //
    //const rapidjson::Value& texture = input.doc["texture"].GetArray();
    //
    //for (rapidjson::SizeType i = 0; i < texture.Size(); i++)
    //{
    //    const rapidjson::Value& coords = texture[i];
    //
    //    TextVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
    //        coords[rapidjson::SizeType(1)].GetDouble()));
    //}
}

void Sphere::InsertModelData()
{
    InsertPosVtx();
    InsertIdxVtx();
    //InsertTextCoord();
}

GLuint Sphere::GetVaoID()
{
    return vaoID;
}

GLuint Sphere::GetVboID()
{
    return vboID;
}

GLuint Sphere::GetEboID()
{
    return eboID;
}

GLenum Sphere::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Sphere::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Sphere::GetDrawCount()
{
    return static_cast<GLuint>(IdxVec.size());
}

void Sphere::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Sphere::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Sphere::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Sphere::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Sphere::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Sphere::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Sphere::CreateVAO()
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

void Sphere::CreateVBO()
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

void Sphere::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
        reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void Sphere::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Sphere::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
}

bool Sphere::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}

GLuint Sphere::GetIndicesCount()
{
    return static_cast<GLuint>(IdxVec.size());
}
