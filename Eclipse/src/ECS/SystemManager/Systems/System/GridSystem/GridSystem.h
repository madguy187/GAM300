#pragma once

#include "../System.h"

namespace Eclipse
{
    class GridSystem : public System
    {
    public:
        static void Init();
        void Update() override;
    };
}