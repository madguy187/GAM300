#include "pch.h"
#include "Torus.h"

Torus::Torus() :
  vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
  primitiveType{ GL_TRIANGLES },
  primitiveCount{ 2 },
  drawCount{ 6 },
  TriCount{ 0 }
{
  initModel();
}

void Torus::initModel()
{
  InsertModelData();
  CreateBuffers();

}

void Torus::InsertPosVtx()
{
  // 16 stacks
  // 32 slices
  // start angle is 0
  // CreateTorus(16, 32, 0, TWO_PI)      

  // getting the angle increment 
  float totalAngle = (TWO_PI - 0);
  float angleIncrement = totalAngle / (float)(16);

  const float R = 0.35f;
  const float r = 0.15f;

  float row = 0.0f, col = 0.0f;
  float alpha = 0.0f, beta = 0.0f;
  float sinAlpha = 0.0f, cosAlpha = 0.0f;

  for (int i = 0; i <= 16; ++i)
  {
    // getting the uv for the row
    row = (float)i / (float)16;

    // getting the sin alpha
    sinAlpha = sinf(alpha);

    // getting the cos alpha
    cosAlpha = cosf(alpha);

    for (int j = 0; j <= 32; ++j)
    {
      // getting the uv for col
      col = (float)j / (float)32;

      // getting the beta angle
      beta = 2.0f * col * PI;

      //Position
      glm::vec3 posVtx{ (R + r * cosf(beta)) * sinAlpha ,  r * sinf(beta) , (R + r * cosf(beta)) * cosAlpha };

      //Center
      glm::vec3 center = { R * sinf(alpha), 0.0f , R * cosf(alpha) };

      //Normal
      glm::vec3 nrmVtx = { posVtx.x - center.x , posVtx.y - center.y , posVtx.z - center.z };
      nrmVtx = { nrmVtx.x / r , nrmVtx.y / r , nrmVtx.z / r };

      // setting the UV
      glm::vec2 texVtx{ row , col };

      //Add vertices
      PosVec.push_back(posVtx);
      NormalVec.push_back(nrmVtx);
      TextVec.push_back(texVtx);
    }
    alpha += angleIncrement;
  }

}

void Torus::InsertIdxVtx()
{
  int i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
  int stride = 32 + 1;

  for (int i = 0; i < 16; ++i)
  {
    int currRow = i * stride;

    for (int j = 0; j < 32; ++j)
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

void Torus::InsertTextCoord()
{
}

void Torus::InsertModelData()
{
  InsertPosVtx();
  InsertIdxVtx();
}

GLuint Torus::GetVaoID()
{
  return vaoID;
}

GLuint Torus::GetVboID()
{
  return vboID;
}

GLuint Torus::GetEboID()
{
  return eboID;
}

GLenum Torus::GetPrimitiveType()
{
  return primitiveType;
}

GLuint Torus::GetPrimitiveCount()
{
  return primitiveCount;
}

GLuint Torus::GetDrawCount()
{
  //return drawCount;
  return IdxVec.size();
}

void Torus::SetVaoID(GLuint id)
{
  this->vaoID = id;
}

void Torus::SetVboID(GLuint id)
{
  this->vboID = id;
}

void Torus::SetEboID(GLuint id)
{
  this->eboID = id;
}

void Torus::SetPrimitiveType(GLenum type)
{
  this->primitiveType = type;
}

void Torus::SetPrimitiveCount(GLuint count)
{
  this->primitiveCount = count;
}

void Torus::SetDrawCount(GLuint count)
{
  this->drawCount = count;
}

void Torus::CreateVAO()
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

void Torus::CreateVBO()
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

void Torus::CreateEBO()
{
  glCreateBuffers(1, &eboID);
  glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
    reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayElementBuffer(vaoID, eboID);
  glBindVertexArray(0);
}

void Torus::CreateBuffers()
{
  CreateVBO();
  CreateVAO();
  CreateEBO();
}

void Torus::DeleteModel()
{
  glDeleteVertexArrays(1, &vaoID);
  glDeleteBuffers(1, &vboID);
  glDeleteBuffers(1, &eboID);
}

bool Torus::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
  return (glm::distance(v0, v1) < EPSILON ||
    glm::distance(v1, v2) < EPSILON ||
    glm::distance(v2, v0) < EPSILON);
}

GLuint Torus::GetIndicesCount()
{
  return IdxVec.size();
}