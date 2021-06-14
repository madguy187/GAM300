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

        ECVec3 ambient{ 0.2f , 0.2f, 0.2f };
        ECVec3 diffuse{ 0.5f,0.5f,0.5f };
        ECVec3 specular{ 0.1f,0.1f,0.1f };
        ECVec3 lightColor{ 1.0f,0.99f,0.0f };
        ECVec3 Color{ 1.0f,0.99f,0 };

        float IntensityStrength = 10.0f ;
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        float radius = 0.0f;

        bool hasTexture = false;
        bool visible = true;

        modelIt modelRef;
        shaderIt shaderRef;
        glm::mat4 modelNDC_xform{ 0 };
    };
}