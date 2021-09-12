#pragma once
#include "pch.h"

namespace Eclipse
{
    struct ParentChildComponent
    {
        Entity parentIndex{ MAX_ENTITY + 1 };
        ECVec3 offset{ 0.0f };
    };
}
