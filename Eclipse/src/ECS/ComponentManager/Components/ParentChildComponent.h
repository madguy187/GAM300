#pragma once
#include "pch.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct ParentChildComponent
    {
        META_DATA(ParentChildComponent);
        Entity parentIndex{ MAX_ENTITY + 1 };
        ECVec3 offset{ 0.0f };
    };
}
