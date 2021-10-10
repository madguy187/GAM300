#pragma once
#include "pch.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "BasicPrimitives/Frustum.h"
#include <any>

namespace Eclipse
{
    class DebugRenderingManager
    {
        std::map<unsigned int, std::any> debugShapes;
        void CheckUniformLoc(Shader* _shdrpgm, CameraComponent& _camera);
    public:
        bool Visible = false;

        void AddCameraFrustum(unsigned int ID);
        void DrawFrustum(unsigned int ID, FrameBufferMode);
        void SetDebugShape(unsigned int ID, std::any newShape);

        void DrawDebugShapes(FrameBufferMode);

        void DeleteDebugShape(unsigned int ID);
        void ClearDebugShapes();
    };
}