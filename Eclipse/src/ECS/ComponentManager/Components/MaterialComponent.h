#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct MaterialComponent
    {
        unsigned int ID = 0;
        ECVec3 ambient{ 0.0f,0.0f,0.0f };
        ECVec3 diffuse{ 0.0f,0.0f,0.0f };
        ECVec3 specular{ 0.1f,0.1f,0.1f };
        float shininess = 0.25f;
        float MaximumShininess = 128.0f;

        // Check Highlighting
        bool RegisterForHighlight = false;
        bool Highlight = false;
        float Thickness = 0.05f;
    };
}