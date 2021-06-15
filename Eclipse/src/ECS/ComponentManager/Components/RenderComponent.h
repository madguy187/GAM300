#pragma once
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct RenderComponent
    {
        using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
        using textureIt = std::map<std::string, Eclipse::Texture>::iterator;
        using shaderIt = std::map<std::string, Shader>::iterator;

        ECVec3 color{ 1.0f, 1.0f, 1.0f };
        ECVec2 textureIdx{ 0.0, 0.0 };

        bool hasTexture = false;
        bool isQuad = false;
        glm::mat4 modelNDC_xform{ 0 };

        modelIt modelRef;
        textureIt textureRef;
        shaderIt shaderRef;

        unsigned int ID = 0;
        std::string name;

        unsigned int newLayer{ 0 };
        unsigned int layerNum{ 0 };

        float transparency{ 1.0f };
        ECVec4 lightColor{ 1.0f,1.0f,1.0f,1.0f };
    };
}