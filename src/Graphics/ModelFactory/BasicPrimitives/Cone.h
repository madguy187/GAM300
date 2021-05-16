#ifndef CONE_H
#define CONE_H

#include "IModel.h"
#include <memory>

class Cone : public IModel
{
public:
  Cone();
  virtual ~Cone() = default;

  void initModel() override;

  void InsertPosVtx();
  void InsertIdxVtx();
  void InsertTextCoord();
  void InsertModelData();

  GLuint GetVaoID() override;
  GLuint GetVboID() override;
  GLuint GetEboID() override;
  GLenum GetPrimitiveType() override;
  GLuint GetPrimitiveCount() override;
  GLuint GetDrawCount() override;

  void SetVaoID(GLuint id) override;
  void SetVboID(GLuint id) override;
  void SetEboID(GLuint id) override;
  void SetPrimitiveType(GLenum type) override;
  void SetPrimitiveCount(GLuint count) override;
  void SetDrawCount(GLuint count) override;

  void CreateVAO() override;
  void CreateVBO() override;
  void CreateEBO() override;
  void CreateBuffers() override;

  void DeleteModel() override;
  bool DegenerateTri(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
  GLuint GetIndicesCount();

private:
  GLuint vaoID;
  GLuint vboID;
  GLuint eboID;
  GLenum primitiveType;
  GLuint primitiveCount;
  GLuint drawCount;
  GLuint TriCount;

  std::vector<glm::vec3> PosVec;
  std::vector<glm::vec3> NormalVec;
  std::vector<glm::vec2> TextVec;
  std::vector<GLushort> IdxVec;
};

#endif /* CONE_H */
