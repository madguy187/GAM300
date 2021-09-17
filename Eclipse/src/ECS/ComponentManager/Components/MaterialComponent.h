#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct MaterialComponent
    {
        MaterialModelType Modeltype = MaterialModelType::MT_NONE;
        ECVec3 ambient{ 0.0f,0.0f,0.0f };
        ECVec3 diffuse{ 1.0f,1.0f,1.0f };
        ECVec3 specular{ 1.0f,1.0f,1.0f };
        ECVec3 HighlightColour{ 1.0f,0.0f,0.0f };
        bool Highlight = false;
        bool NoTextures = true;
        float shininess = 0.25f;
        float MaximumShininess = 128.0f;

        /*************************************************************************
          Components for Highlighting
        *************************************************************************/
        float Thickness = 0.05f; // Check Highlighting
        float ScaleUp = 1.05f; // Only for 3D Primitives

        //TODO
        //ComboIndex if for imgui combolist to select the correct model type - Tian Yu
        size_t ComboIndex;

        /*************************************************************************
          Components for a Texture
        *************************************************************************/

        //TODO
        //ComboIndex if for imgui combolist to select the correct model type - Tian Yu
        size_t TextureComboIndex;
        TextureType Type = TextureType::TT_UNASSIGNED;

        // Basic Meshes
        bool hasTexture = false;
        std::string TextureRef;

        // Models
        std::string TextureKey;
        std::map<unsigned int, std::vector<Texture>> HoldingTextures;
    };
}