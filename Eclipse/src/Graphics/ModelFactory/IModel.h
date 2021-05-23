/**********************************************************************************
* \file            IModel.h
*
* \brief        Definition of the Model interface class class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			10 oct 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef IMODEL_H
#define IMODEL_H

#include "glew.h"
#include "glm.hpp"
#include "Parser.h"
#include <string>
#include <vector>
#include "ModelGlobal.h"
#include "Math_Lib.h"

class IModel
{
public:
    virtual ~IModel() = default;

    virtual void initModel() = 0;

    virtual GLuint GetVaoID() = 0;
    virtual GLuint GetVboID() = 0;
    virtual GLuint GetEboID() = 0;
    virtual GLenum GetPrimitiveType() = 0;
    virtual GLuint GetPrimitiveCount() = 0;
    virtual GLuint GetDrawCount() = 0;

    virtual void SetVaoID(GLuint id) = 0;
    virtual void SetVboID(GLuint id) = 0;
    virtual void SetEboID(GLuint id) = 0;
    virtual void SetPrimitiveType(GLenum type) = 0;
    virtual void SetPrimitiveCount(GLuint count) = 0;
    virtual void SetDrawCount(GLuint count) = 0;

    virtual void CreateVAO() = 0;
    virtual void CreateVBO() = 0;
    virtual void CreateEBO() = 0;
    virtual void CreateBuffers() = 0;

    virtual void DeleteModel() = 0;
};

#endif /* IMODEL_H */
