#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct PointLightComponent
    {
        using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
        using textureIt = std::map<std::string, Eclipse::Texture>::iterator;
        using shaderIt = std::map<std::string, Shader>::iterator;

        unsigned int ID = 0;

        ECVec3 ambient;
        ECVec3 diffuse;
        ECVec3 specular;

        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec3 position{ 1.0f,1.0f,1.0f };

        float constant = 0.0f;
        float linear = 0.0f;
        float quadratic = 0.0f;
        float radius = 0.0f;

        bool hasTexture = false;
        bool visible;

        modelIt modelRef;
        textureIt textureRef;
        shaderIt shaderRef;

        glm::mat4 modelNDC_xform{ 0 };
    };
}