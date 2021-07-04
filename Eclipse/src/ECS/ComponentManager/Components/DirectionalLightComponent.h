#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct DirectionalLightComponent
    {
        unsigned int ID = 0;
        bool visible = false;

        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec3 Direction{ -0.2f,-1.0f,-0.3f };
        ECVec3 ambient{ 1.0f , 1.0f, 1.0f };
        ECVec3 diffuse{ 0.4f,0.4f,0.4f };
        ECVec3 specular{ 0.5f,0.5f,0.5f };
        ECVec4 Color{ 1.0f,1.0f,1.0f,1 };

        IModel* modelRef;
        Shader* shaderRef;
        Texture* textureRef;

        glm::mat4 modelNDC_xform{ 0 };
    };
}