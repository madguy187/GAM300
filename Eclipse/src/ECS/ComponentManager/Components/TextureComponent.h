#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct TextureComponent
    {
        unsigned int ID = 0;
        TextureType Type = TextureType::TT_UNASSIGNED;
        std::string TextureKey;
        std::vector<Texture> Textures;
    };
}