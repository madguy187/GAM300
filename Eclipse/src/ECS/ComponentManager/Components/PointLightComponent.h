#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct PointLightComponent
    {
        unsigned int ID = 0;

        ECVec3 ambient{ 0.2f , 0.2f, 0.2f };
        ECVec3 diffuse{ 0.5f,0.5f,0.5f };
        ECVec3 specular{ 0.1f,0.1f,0.1f };
        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec4 Color{ 1.0f,1.0f,1.0f,1.0f };

        float IntensityStrength = 10.0f;
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        float radius = 0.0f;
        int AttenuationLevel = 5;

        bool hasTexture = false;
        bool visible = true;

        IModel* modelRef;
        Shader* shaderRef;
        Texture* textureRef;
        glm::mat4 modelNDC_xform{ 0 };
    };
}
