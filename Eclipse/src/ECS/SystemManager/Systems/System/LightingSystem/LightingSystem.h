#pragma once

#include "../System.h"

namespace Eclipse
{
    class LightingSystem : public System
    {
    public:
        static void Init();
        static inline Entity Hand;

    private:
        void Update() override;
    };
}