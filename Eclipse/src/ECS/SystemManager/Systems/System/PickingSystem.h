#pragma once

#include "System.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

namespace Eclipse
{
    class PickingSystem : public System
    {
    public:
        static void Init();
        void Update() override;
    };
}