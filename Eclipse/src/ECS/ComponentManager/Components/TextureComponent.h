#pragma once
#include "pch.h"
#include "Library/Math/Vector.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct TextureComponent
    {
        META_DATA(TextureComponent);
        std::string textureRef;
    };
}