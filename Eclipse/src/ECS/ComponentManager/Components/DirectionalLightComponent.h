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

        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec3 Direction{ 0.0f,0.0f,0.0f };
        ECVec3 ambient;
        ECVec3 diffuse;
        ECVec3 specular;
        bool visible;
        modelIt modelRef;
        shaderIt shaderRef;
        glm::mat4 modelNDC_xform{ 0 };
    };
}