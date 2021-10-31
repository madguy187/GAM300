#pragma once
#include "Graphics/Instancing/Box.h"

namespace Eclipse
{
    class DebugManager
    {
    public:
        AABB_ DebugBoxes;

    public:
        DebugManager();
        void ResetInstancedDebugBoxes();
        void Render();
    };
}