#include "pch.h"
#include "../include/Pyramid.h"

Pyramid::Pyramid() :
  vaoID{ 1 }, vboID{ 1 }, eboID{ 1 },
  primitiveType{ GL_TRIANGLES },
  primitiveCount{ 2 },
  drawCount{ 6 },
  TriCount{ 0 }
{
  initModel();
}

void Pyramid::initModel()
{
  InsertModelData();
  CreateBuffers();

}

void Pyramid::InsertPosVtx()
{
  glm::vec3 const Faces[] =
  {
      glm::vec3(-0.5f,  0.0f,  0.5f),
      glm::vec3(-0.5f, 0.0f,  -0.5f),
      glm::vec3(0.5f,  0.0f,  -0.5f),
      glm::vec3(0.5f,  0.0f,  0.5f),
      glm::vec3(0.0f, 0.5f,  0.0f)
  };

  glm::vec2 const TextureCoordiantes[] =
  {
      glm::vec2(0.0f,0.0f),
      glm::vec2(5.0f,0.0f),
      glm::vec2(0.0f,0.0f),
  };

  for (int i = 0; i <= 6; ++i)
  {
    PosVec.push_back(Faces[i]);
    TextVec.push_back(TextureCoordiantes[i]);
  }
}

void Pyramid::InsertIdxVtx()
{
  int _indices[] =
  {
    0,1,2,
    0,2,3,
    0,1,4,
    1,2,4,
    2,3,4,
    3,0,4
  };

  for (int i = 0; i <= 18; i++)
  {
    IdxVec.push_back(_indices[i]);
  }
}

void Pyramid::InsertTextCoord()
{
}

void Pyramid::InsertModelData()
{
  InsertPosVtx();
  InsertIdxVtx();
}

GLuint Pyramid::GetVaoID()
{
  return vaoID;
}

GLuint Pyramid::GetVboID()
{
  return vboID;
}

GLuint Pyramid::GetEboID()
{
  return eboID;
}

GLenum Pyramid::GetPrimitiveType()
{
  return primitiveType;
}

GLuint Pyramid::GetPrimitiveCount()
{
  return primitiveCount;
}

GLuint Pyramid::GetDrawCount()
{
  //return drawCount;
  return IdxVec.size();
}

void Pyramid::SetVaoID(GLuint id)
{
  this->vaoID = id;
}

void Pyramid::SetVboID(GLuint id)
{
  this->vboID = id;
}

void Pyramid::SetEboID(GLuint id)
{
  this->eboID = id;
}

void Pyramid::SetPrimitiveType(GLenum type)
{
  this->primitiveType = type;
}

void Pyramid::SetPrimitiveCount(GLuint count)
{
  this->primitiveCount = count;
}

void Pyramid::SetDrawCount(GLuint count)
{
  this->drawCount = count;
}

void Pyramid::CreateVAO()
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

void Pyramid::CreateVBO()
{
  glCreateBuffers(1, &vboID);
  glNamedBufferStorage(vboID, sizeof(glm::vec3) * PosVec.size() + sizeof(glm::vec2) * TextVec.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferSubData(vboID, 0, sizeof(glm::vec3) * PosVec.size(), PosVec.data());
  glNamedBufferSubData(vboID, sizeof(glm::vec3) * PosVec.size(), sizeof(glm::vec2) * TextVec.size(), TextVec.data());
}

void Pyramid::CreateEBO()
{
  glCreateBuffers(1, &eboID);
  glNamedBufferStorage(eboID, sizeof(GLushort) * IdxVec.size(),
    reinterpret_cast<GLvoid*>(IdxVec.data()), GL_DYNAMIC_STORAGE_BIT);
  glVertexArrayElementBuffer(vaoID, eboID);
  glBindVertexArray(0);
}

void Pyramid::CreateBuffers()
{
  CreateVBO();
  CreateVAO();
  CreateEBO();
}

void Pyramid::DeleteModel()
{
  glDeleteVertexArrays(1, &vaoID);
  glDeleteBuffers(1, &vboID);
  glDeleteBuffers(1, &eboID);
}

bool Pyramid::DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
  return (glm::distance(v0, v1) < EPSILON ||
    glm::distance(v1, v2) < EPSILON ||
    glm::distance(v2, v0) < EPSILON);
}

GLuint Pyramid::GetIndicesCount()
{
  return IdxVec.size();
}