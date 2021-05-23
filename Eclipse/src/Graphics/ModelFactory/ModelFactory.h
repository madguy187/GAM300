/**********************************************************************************
* \file            ModelFactory.h
*
* \brief        Definition of the ModelFactory  class class function(s).
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
#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "IModel.h"
#include "BasicPrimitives/Quad.h"
#include "BasicPrimitives/Triangle.h"
#include "BasicPrimitives/Circle.h"
#include "BasicPrimitives/Lines.h"
#include "BasicPrimitives/LightQuad.h"
#include "BasicPrimitives/Sphere.h"
#include "BasicPrimitives/Plane.h"
#include "BasicPrimitives/Cube.h"
#include "BasicPrimitives/Cylinder.h"
#include "BasicPrimitives/Cone.h"
#include "BasicPrimitives/Torus.h"
#include "BasicPrimitives/Pyramid.h"

#include <memory>

class ModelFactory 
{
public:
    static std::unique_ptr<IModel> create(unsigned int index);
};

#endif /* MODELFACTORY_H */
