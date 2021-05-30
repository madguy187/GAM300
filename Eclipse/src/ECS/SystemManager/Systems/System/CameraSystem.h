#pragma once

#include "System.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/InputHandler/InputWrapper.h"

namespace Eclipse
{
    class CameraSystem : public System
    {
    public:
        InputWrapper InputHandler;

        static void Init();
        void Update() override;
    };
}