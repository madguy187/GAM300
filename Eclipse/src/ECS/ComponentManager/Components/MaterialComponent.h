#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct MaterialComponent
    {
        unsigned int ID = 0;
        ECVec3 ambient{ 0.2f , 0.2f, 0.2f };
        ECVec3 diffuse{ 0.5f,0.5f,0.5f };
        ECVec3 specular{ 0.1f,0.1f,0.1f };
        float shininess = 0.0f;
        ECVec3 Color{ 0,0,0 };
        bool hasTexture = false;
        bool visible = true;
    };
}