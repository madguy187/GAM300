#pragma once

#include "Global.h"

namespace Eclipse
{
    struct Test1
    {
        int x = 1;
    };

    struct DestroyEvent
    {
        Entity ID{ MAX_ENTITY };
        bool IsDead{ false };
    };
}
