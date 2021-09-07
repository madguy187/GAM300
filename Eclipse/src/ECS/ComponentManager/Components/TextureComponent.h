#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct TextureComponent
    {
        // See if need or not
        unsigned int ID = 0;
        TextureType Type = TextureType::TT_UNASSIGNED;

        // Assimp Models
        std::string TextureKey;
        std::map<unsigned int, std::vector<Texture>> test;
    };
}