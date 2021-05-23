/**********************************************************************************
* \file         Sphere.h
*
* \brief        Definition of the Sphere class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			03 May 2021
*
* \copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef SPHERE_H
#define SPHERE_H

#define _USE_MATH_DEFINES

#include "IModel.h"
#include <memory>
#include <cmath>

class Sphere : public IModel
{
public:
    Sphere();
    virtual ~Sphere() = default;

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

#endif /* SPHERE_H */
