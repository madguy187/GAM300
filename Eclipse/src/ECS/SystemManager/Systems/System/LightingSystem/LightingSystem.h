#pragma once

#include "../System.h"

namespace Eclipse
{
    class LightingSystem : public System
    {
    public:
        static void Init();

    private:
        void Update() override;
    };
}