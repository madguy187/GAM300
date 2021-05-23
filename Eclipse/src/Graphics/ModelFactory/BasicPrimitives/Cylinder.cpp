#include "pch.h"
#include "Cylinder.h"

Cylinder::Cylinder() :
  vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
  primitiveType{ GL_TRIANGLES },
  primitiveCount{ 2 },
  drawCount{ 6 },
  TriCount{ 0 }
{
  initModel();
}

void Cylinder::initModel()
{
  InsertModelData();
  CreateBuffers();

}

void Cylinder::InsertPosVtx()
{
  const float radius = 0.5f; //Radius size

  float row = 0.0f;
  float col = 0.0f;
  float alpha = 0.0f;

  for (int stack = 0; stack <= 1; ++stack)
  {
    row = (float)stack / 1;

    for (int slice = 0; slice <= 8; ++slice)
    {
      col = (float)slice / 8;
      alpha = 2.0f * col * PI;

      //Position
      glm::vec3 posVtx{ radius * sinf(alpha) , radius - row , radius * cosf(alpha) };

      //Center of Cylinder
      glm::vec3 center = Vec3{ 0.0f, posVtx.y, 0.0f };

      //Normal
      glm::vec3 nrmVtx{ posVtx.x - center.x , posVtx.y - center.y , posVtx.z - center.z };
      nrmVtx = { nrmVtx.x / radius , nrmVtx.y / radius , nrmVtx.z / radius };

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
  const float deltaAlpha = (2.0f * PI ) / 8;

  //Caps
  for (int i = 0, j = 0; i < 8; ++i, j += 6)
  {
    col = static_cast<float>(i) / 8;
    alpha = col * 2.0f * PI ;

    //TopCap
    //Position
    glm::vec3 v1_posVtx{ 0.0f , 0.5f , 0.0f };
    glm::vec3 v2_posVtx{ radius * sinf(alpha), 0.5f, radius * cos(alpha) };
    glm::vec3 v3_posVtx{ radius * sinf(alpha + deltaAlpha), 0.5f, radius * cos(alpha + deltaAlpha) };

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
    IdxVec.push_back(18 + j);
    IdxVec.push_back(18 + j + 1);
    IdxVec.push_back(18 + j + 2);

    //CapBottom
    //Position
    glm::vec3 v4_posVtx{ 0.0f , -0.5f , 0.0f };
    glm::vec3 v5_posVtx{ -radius * sinf(alpha), -0.5f, radius * cos(alpha) };
    glm::vec3 v6_posVtx{ -radius * sinf(alpha + deltaAlpha), -0.5f, radius * cos(alpha + deltaAlpha) };

    //Normal
    glm::vec3 v4_nrmVtx = v4_posVtx;
    glm::vec3 v5_nrmVtx = v4_posVtx;
    glm::vec3 v6_nrmVtx = v4_posVtx;

    //UV
    glm::vec2 v4_texVtx{ v4_posVtx.x + 0.5f , v4_posVtx.z + 0.5f };
    glm::vec2 v5_texVtx{ v5_posVtx.x + 0.5f , v5_posVtx.z + 0.5f };
    glm::vec2 v6_texVtx{ v6_posVtx.x + 0.5f , v6_posVtx.z + 0.5f };

    //Round decimal
    v4_posVtx = RoundDecimal(v4_posVtx);
    v5_posVtx = RoundDecimal(v5_posVtx);
    v6_posVtx = RoundDecimal(v6_posVtx);

    v4_nrmVtx = RoundDecimal(v4_nrmVtx);
    v5_nrmVtx = RoundDecimal(v5_nrmVtx);
    v6_nrmVtx = RoundDecimal(v6_nrmVtx);

    //Add vertices
    PosVec.push_back(v4_posVtx);
    PosVec.push_back(v5_posVtx);
    PosVec.push_back(v6_posVtx);

    NormalVec.push_back(v4_nrmVtx);
    NormalVec.push_back(v5_nrmVtx);
    NormalVec.push_back(v6_nrmVtx);

    TextVec.push_back(v4_texVtx);
    TextVec.push_back(v5_texVtx);
    TextVec.push_back(v6_texVtx);

    //Add index
    IdxVec.push_back(18 + j + 3);
    IdxVec.push_back(18 + j + 4);
    IdxVec.push_back(18 + j + 5);
  }
}

void Cylinder::InsertIdxVtx()
{
  int i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
  int stride = 8 + 1;

  for (int i = 0; i < 1; ++i)
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
        IdxVec.push_back(i2);
        IdxVec.push_back(i1);
        IdxVec.push_back(i0);
        ++TriCount;
      }

      /*  You need to compute the indices for the second triangle here */
      i3 = i2;
      i4 = i3 - 1;
      i5 = i0;;

      /*  Ignore degenerate triangle */
      if (!DegenerateTri(PosVec[i3], PosVec[i4], PosVec[i5]))
      {
        IdxVec.push_back(i5);
        IdxVec.push_back(i4);
        IdxVec.push_back(i3);
        ++TriCount;
      }
    }
  }
}

void Cylinder::InsertTextCoord()
{
}

void Cylinder::InsertModelData()
{
  InsertPosVtx();
}

GLuint Cylinder::GetVaoID()
{
  return vaoID;
}

GLuint Cylinder::GetVboID()
{
  return vboID;
}

GLuint Cylinder::GetEboID()
{
  return eboID;
}

GLenum Cylinder::GetPrimitiveType()
{
  return primitiveType;
}

GLuint Cylinder::GetPrimitiveCount()
{
  return primitiveCount;
}

GLuint Cylinder::GetDrawCount()
{
  //return drawCount;
  return IdxVec.size();
}

void Cylinder::SetVaoID(GLuint id)
{
  this->vaoID = id;
}

void Cylinder::SetVboID(GLuint id)
{
  this->vboID = id;
}

void Cylinder::SetEboID(GLuint id)
{
  this->eboID = id;
}

void Cylinder::SetPrimitiveType(GLenum type)
{
  this->primitiveType = type;
}

void Cylinder::SetPrimitiveCount(GLuint count)
{
  this->primitiveCount = count;
}

void Cylinder::SetDrawCount(GLuint count)
{
  this->drawCount = count;
}

void Cylinder::CreateVAO()
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

void Cylinder::CreateVBO()
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

void Cylinder::CreateEBO()
{
  glCreateBuffers(1, &eboID);
  glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
    reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayElementBuffer(vaoID, eboID);
  glBindVertexArray(0);
}

void Cylinder::CreateBuffers()
{
  CreateVBO();
  CreateVAO();
  CreateEBO();
}

void Cylinder::DeleteModel()
{
  glDeleteVertexArrays(1, &vaoID);
  glDeleteBuffers(1, &vboID);
  glDeleteBuffers(1, &eboID);
}

bool Cylinder::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
  return (glm::distance(v0, v1) < EPSILON ||
    glm::distance(v1, v2) < EPSILON ||
    glm::distance(v2, v0) < EPSILON);
}

GLuint Cylinder::GetIndicesCount()
{
  return IdxVec.size();
}