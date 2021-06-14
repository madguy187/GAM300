#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct DirectionalLightComponent
    {
        using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
        using shaderIt = std::map<std::string, Shader>::iterator;

        unsigned int ID = 0;
        bool visible = false;

        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec3 Direction{ 0.0f,0.0f,6.0f };
        ECVec3 ambient{ 1.0f , 1.0f, 1.0f };
        ECVec3 diffuse{ 0.5f,0.5f,0.5f };
        ECVec3 specular{ 1.0f,1.0f,1.0f };

        glm::mat4 modelNDC_xform{ 0 };
    };
}