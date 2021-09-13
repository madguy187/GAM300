#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct MaterialComponent
    {
        enum class ModelType
        {
            None = 0,
            BasicPrimitives = 1,
            Models3D = 2,
            Maxcount
        };  
    
        ModelType Modeltype = ModelType::None;
        ECVec3 ambient{ 0.0f,0.0f,0.0f };
        ECVec3 diffuse{ 1.0f,1.0f,1.0f };
        ECVec3 specular{ 1.0f,1.0f,1.0f };
        ECVec3 HighlightColour{ 1.0f,0.0f,0.0f };
        float shininess = 0.25f;
        float MaximumShininess = 128.0f;
        bool RegisterForHighlight = false;
        bool Highlight = false;
        bool NoTextures = true;

        // Check Highlighting
        float Thickness = 0.05f;

        // Only for 3D Primitives
        float ScaleUp = 1.05f;

        //TODO
        //ComboIndex if for imgui combolist to select the correct model type - Tian Yu
        size_t ComboIndex;

    };
}