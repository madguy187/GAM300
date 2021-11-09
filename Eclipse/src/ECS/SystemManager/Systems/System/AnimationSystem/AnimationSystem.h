#pragma once

#include "../System.h"
#include "Graphics/Animation/AnimationManager.h"

namespace Eclipse
{
    class AnimationSystem : public System
    {
    public:
        static void Init();
        void Update() override;
    };
}