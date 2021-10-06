#include "pch.h"
#include "Cone.h"

Cone::Cone() :
    vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
    primitiveType{ GL_TRIANGLES },
    primitiveCount{ 2 },
    drawCount{ 6 },
    TriCount{ 0 }
{
    initModel();
}

void Cone::initModel()
{
    InsertModelData();
    CreateBuffers();

}

void Cone::InsertPosVtx()
{
    // 16 stack , 8 slices

    //Radius size
    const float radius = 0.5f;

    //Initial Loop
    float row = 0.0f, col = 0.0f;
    float alpha = 0.0f;

    for (int stack = 0; stack <= 16; ++stack)
    {
        row = static_cast<float>(stack) / 16;

        for (size_t slice = 0; slice <= 8; ++slice)
        {
            col = static_cast<float>(slice) / 8;
            alpha = 2.0f * col * PI;

            //Position
            glm::vec3 posVtx{ radius * row * sinf(alpha) , -row + 0.5f , radius * row * cosf(alpha) };

            //Normal
            glm::vec3 nrmVtx{ sinf(alpha) , 0.5f , cosf(alpha) };

            //UV
            glm::vec2 texVtx{ col , row };

            //Round decimal
            posVtx = RoundDecimal(posVtx);
            nrmVtx = RoundDecimal(nrmVtx);

            PosVec.push_back(posVtx);
            NormalVec.push_back(nrmVtx);
            TextVec.push_back(texVtx);
        }
    }

    InsertIdxVtx();

    //Const values
    const float deltaAlpha = (2.0f * PI) / 8;
    const int totalV = 153;

    //Cap
    for (int i = 0, j = 0; i < 8; ++i, j += 3)
    {
        col = static_cast<float>(i) / 8; //Column
        alpha = col * 2.0f * PI; //Update alpha

        //CapBottom
        //Position
        glm::vec3 v1_posVtx{ 0.f , -0.5f , 0.0f };
        glm::vec3 v2_posVtx{ -radius * sinf(alpha), -0.5f, radius * cos(alpha) };
        glm::vec3 v3_posVtx{ -radius * sinf(alpha + deltaAlpha), -0.5f, radius * cos(alpha + deltaAlpha) };

        //Normal
        glm::vec3 v1_nrmVtx = v1_posVtx;
        glm::vec3 v2_nrmVtx = v1_posVtx;
        glm::vec3 v3_nrmVtx = v1_posVtx;

        //UV
        glm::vec2 v1_texVtx{ v1_posVtx.x + 0.5f , v1_posVtx.z + 0.5f };
        glm::vec2 v2_texVtx{ v2_posVtx.x + 0.5f , v2_posVtx.z + 0.5f };
        glm::vec2 v3_texVtx{ v3_posVtx.x + 0.5f , v3_posVtx.z + 0.5f };

        //Round decimal
        v1_posVtx = RoundDecimal(v1_posVtx);
        v2_posVtx = RoundDecimal(v2_posVtx);
        v3_posVtx = RoundDecimal(v3_posVtx);

        v1_nrmVtx = RoundDecimal(v1_nrmVtx);
        v2_nrmVtx = RoundDecimal(v2_nrmVtx);
        v3_nrmVtx = RoundDecimal(v3_nrmVtx);

        //Add vertices
        PosVec.push_back(v1_posVtx);
        PosVec.push_back(v2_posVtx);
        PosVec.push_back(v3_posVtx);

        NormalVec.push_back(v1_nrmVtx);
        NormalVec.push_back(v2_nrmVtx);
        NormalVec.push_back(v3_nrmVtx);

        TextVec.push_back(v1_texVtx);
        TextVec.push_back(v2_texVtx);
        TextVec.push_back(v3_texVtx);

        //Add index
        IdxVec.push_back(static_cast<GLushort>(totalV + j));
        IdxVec.push_back(static_cast<GLushort>(totalV + j + 1));
        IdxVec.push_back(static_cast<GLushort>(totalV + j + 2));
    }
}

void Cone::InsertIdxVtx()
{
    int i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
    int stride = 8 + 1;

    for (int i = 0; i < 16; ++i)
    {
        int currRow = i * stride;

        for (int j = 0; j < 8; ++j)
        {
            /*  You need to compute the indices for the first triangle here */
            i0 = currRow + j;
            i1 = i0 + 1;
            i2 = i1 + stride;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(PosVec[i0], PosVec[i1], PosVec[i2]))
            {
                IdxVec.push_back(static_cast<GLushort>(i2));
                IdxVec.push_back(static_cast<GLushort>(i1));
                IdxVec.push_back(static_cast<GLushort>(i0));
                ++TriCount;
            }

            /*  You need to compute the indices for the second triangle here */
            i3 = i2;
            i4 = i3 - 1;
            i5 = i0;;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(PosVec[i3], PosVec[i4], PosVec[i5]))
            {
                IdxVec.push_back(static_cast<GLushort>(i5));
                IdxVec.push_back(static_cast<GLushort>(i4));
                IdxVec.push_back(static_cast<GLushort>(i3));
                ++TriCount;
            }
        }
    }
}

void Cone::InsertTextCoord()
{
}

void Cone::InsertModelData()
{
    InsertPosVtx();
}

GLuint Cone::GetVaoID()
{
    return vaoID;
}

GLuint Cone::GetVboID()
{
    return vboID;
}

GLuint Cone::GetEboID()
{
    return eboID;
}

GLenum Cone::GetPrimitiveType()
{
    return primitiveType;
}

GLuint Cone::GetPrimitiveCount()
{
    return primitiveCount;
}

GLuint Cone::GetDrawCount()
{
    //return drawCount;
    return static_cast<GLuint>(IdxVec.size());
}

void Cone::SetVaoID(GLuint id)
{
    this->vaoID = id;
}

void Cone::SetVboID(GLuint id)
{
    this->vboID = id;
}

void Cone::SetEboID(GLuint id)
{
    this->eboID = id;
}

void Cone::SetPrimitiveType(GLenum type)
{
    this->primitiveType = type;
}

void Cone::SetPrimitiveCount(GLuint count)
{
    this->primitiveCount = count;
}

void Cone::SetDrawCount(GLuint count)
{
    this->drawCount = count;
}

void Cone::CreateVAO()
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

void Cone::CreateVBO()
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

void Cone::CreateEBO()
{
    glCreateBuffers(1, &eboID);
    glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
        reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoID, eboID);
    glBindVertexArray(0);
}

void Cone::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateEBO();
}

void Cone::DeleteModel()
{
    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &eboID);
}

bool Cone::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}

GLuint Cone::GetIndicesCount()
{
    return static_cast<GLuint>(IdxVec.size());
}