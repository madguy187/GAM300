/**********************************************************************************
* \file         Lines3D.h
*
* \brief        Definition of functions of Lines3D.cpp
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			02 June 2021
*
* \copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
*               or disclosure of this file or its contents without the prior
*               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef LINES3D_H
#define LINES3D_H

#include "IModel.h"
#include <memory>

class Lines3D : public IModel
{
public:
    Lines3D(glm::vec3 _startPos = glm::vec3{ -0.5f, -0.0f, 0.0f }, glm::vec3 _endPos = glm::vec3{ 0.5f, 0.0f, 0.0f });
    virtual ~Lines3D() = default;

    void initModel() override;

    void InsertPosVtx();
    void InsertIdxVtx();
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
private:
    GLuint vaoID;
    GLuint vboID;
    GLuint eboID;
    GLenum primitiveType;
    GLuint primitiveCount;
    GLuint drawCount;

    glm::vec3 startPos;
    glm::vec3 endPos;

    std::vector<glm::vec3> PosVec;
    std::vector<glm::vec2> TextVec;
    std::vector<GLushort> IdxVec;
};

#endif /* LINES3D_H */

