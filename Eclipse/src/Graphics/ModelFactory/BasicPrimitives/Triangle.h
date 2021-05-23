/**********************************************************************************
* \file         Traignle.h
*
* \brief        Definition of functions of Triangle.cpp
*
* \author       Lin Yanping Rachel
*
* \email		    l.yanpingrachel@digipen.edu
*
* \date			    9 December 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*               or disclosure of this file or its contents without the prior
*               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "IModel.h"

class Triangle : public IModel
{
public:
    Triangle();
    virtual ~Triangle() = default;

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

    std::vector<glm::vec2> PosVec;
    std::vector<glm::vec2> TextVec;
    std::vector<GLushort> IdxVec;
};

#endif /* TRIANGLE_H */

