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
#include "Quad.h"
#include "Triangle.h"
#include "Circle.h"
#include "Lines.h"
#include "LightQuad.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Torus.h"
#include "Pyramid.h"
#include "Lines3D.h"

#include <memory>

class ModelFactory 
{
public:
    static std::unique_ptr<IModel> create(unsigned int index);
};

#endif /* MODELFACTORY_H */
