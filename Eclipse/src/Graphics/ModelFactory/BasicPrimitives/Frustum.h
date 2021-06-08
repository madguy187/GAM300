#pragma once
/**********************************************************************************
* \file         Frustum.h
*
* \brief        Definition of functions of Frustum.cpp
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			03 June 2021
*
* \copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
*               or disclosure of this file or its contents without the prior
*               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "IModel.h"
#include <memory>

class Frustum
{
public:
    Frustum();

    glm::vec3 ComputeNearCenter(CameraComponent& _camera);
    glm::vec3 ComputeFarCenter(CameraComponent& _camera);
    glm::vec2 ComputeNearPlaneXY(CameraComponent& _camera);
    glm::vec2 ComputeFarPlaneXY(CameraComponent& _camera);

    using LineArr = std::array<Lines3D, 12>;

    void AddLineSegment(unsigned int index, glm::vec3 p0, glm::vec3 p1);
    void UpdateFrustum(CameraComponent& _camera);

    LineArr& GetLineSegments();
private:
    LineArr lineSegments;
};

#endif /* FRUSTUM_H */

