#include "pch.h"
#include "../include/Plane.h"

Plane_::Plane_() :
  vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
  primitiveType{ GL_TRIANGLES },
  primitiveCount{ 2 },
  drawCount{ 6 },
  TriCount{ 0 }
{
  initModel();
}

void Plane_::initModel()
{
  InsertModelData();
  CreateBuffers();
}

void Plane_::InsertPosVtx()
{
  for (int stack = 0; stack <= 1; ++stack)
  {
    float row = (float)stack / 1;

    for (int slice = 0; slice <= 1; ++slice)
    {
      float col = (float)slice / 1;

      glm::vec3 posVtx { col - 0.5f , 0.5f - row , 0.0f };
      glm::vec3 nrmVtx { 0.0f , 0.0f , 1.0f };
      glm::vec2 texVtx { col , row };

      PosVec.push_back(posVtx);
      NormalVec.push_back(nrmVtx);
      TextVec.push_back(texVtx);
    }
  }
}

void Plane_::InsertIdxVtx()
{
  int i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;

  int stride = 1 + 1;

  for (int i = 0; i < 1; ++i)
  {
    int currRow = i * stride;

    for (int j = 0; j < 1; ++j)
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
      if (!DegenerateTri(PosVec[i3],
        PosVec[i4],
        PosVec[i5]))
      {
        IdxVec.push_back(i5);
        IdxVec.push_back(i4);
        IdxVec.push_back(i3);

        ++TriCount;
      }
    }
  }
}

void Plane_::InsertTextCoord()
{
  Parser input;
  input.ParseFile("../meshes/square.json");

  const rapidjson::Value& texture = input.doc["texture"].GetArray();

  for (rapidjson::SizeType i = 0; i < texture.Size(); i++)
  {
    const rapidjson::Value& coords = texture[i];

    TextVec.push_back(glm::vec2(coords[rapidjson::SizeType(0)].GetDouble(),
      coords[rapidjson::SizeType(1)].GetDouble()));
  }
}

void Plane_::InsertModelData()
{
  InsertPosVtx();
  InsertIdxVtx();
}

GLuint Plane_::GetVaoID()
{
  return vaoID;
}

GLuint Plane_::GetVboID()
{
  return vboID;
}

GLuint Plane_::GetEboID()
{
  return eboID;
}

GLenum Plane_::GetPrimitiveType()
{
  return primitiveType;
}

GLuint Plane_::GetPrimitiveCount()
{
  return primitiveCount;
}

GLuint Plane_::GetDrawCount()
{
  return drawCount;
}

void Plane_::SetVaoID(GLuint id)
{
  this->vaoID = id;
}

void Plane_::SetVboID(GLuint id)
{
  this->vboID = id;
}

void Plane_::SetEboID(GLuint id)
{
  this->eboID = id;
}

void Plane_::SetPrimitiveType(GLenum type)
{
  this->primitiveType = type;
}

void Plane_::SetPrimitiveCount(GLuint count)
{
  this->primitiveCount = count;
}

void Plane_::SetDrawCount(GLuint count)
{
  this->drawCount = count;
}

void Plane_::CreateVAO()
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

void Plane_::CreateVBO()
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

void Plane_::CreateEBO()
{
  glCreateBuffers(1, &eboID);
  glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
    reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayElementBuffer(vaoID, eboID);
  glBindVertexArray(0);
}

void Plane_::CreateBuffers()
{
  CreateVBO();
  CreateVAO();
  CreateEBO();
}

void Plane_::DeleteModel()
{
  glDeleteVertexArrays(1, &vaoID);
  glDeleteBuffers(1, &vboID);
  glDeleteBuffers(1, &eboID);
}

bool Plane_::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
  return (glm::distance(v0, v1) < EPSILON ||
    glm::distance(v1, v2) < EPSILON ||
    glm::distance(v2, v0) < EPSILON);
}

GLuint Plane_::GetIndicesCount()
{
  return IdxVec.size();
}
