#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct TextureComponent
    {
        //TODO
        //ComboIndex if for imgui combolist to select the correct texture type - Tian Yu
        size_t ComboIndex = 0;

        // See if need or not
        unsigned int ID = 0;
        TextureType Type = TextureType::TT_UNASSIGNED;

        // Assimp Models
        std::string TextureKey;
        std::map<unsigned int, std::vector<Texture>> HoldingTextures;

        // Basic Primitives
        bool hasTexture = false;
        std::string textureRef;
    };
}