#pragma once

#include "../System.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/InputHandler/LogicalInput.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

namespace Eclipse
{
    class CameraSystem : public System
    {
    public:
        static void Init();
        void Update() override;
    };
}