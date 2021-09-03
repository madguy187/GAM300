#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct MaterialComponent
    {
        enum ModelType
        {
            None = 0,
            BasicPrimitives = 1,
            Models3D = 2,
            Maxcount
        };  

        unsigned int ID = 0;
        ECVec3 ambient{ 0.0f,0.0f,0.0f };
        ECVec3 diffuse{ 0.0f,0.0f,0.0f };
        ECVec3 specular{ 0.1f,0.1f,0.1f };
        ECVec3 HighlightColour{ 1.0f,0.0f,0.0f };
        float shininess = 0.25f;
        float MaximumShininess = 128.0f;
        bool RegisterForHighlight = false;
        bool Highlight = false;
        ModelType Modeltype = ModelType::None;

        // Check Highlighting
        float Thickness = 0.05f;

        // Only for 3D Primitives
        float ScaleUp = 1.05f;
    };
}