#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct MeshComponent
    {
        META_DATA(MeshComponent);
        //--------------------------------------
        // 3D Models
        //--------------------------------------
        using MeshNameType = std::array<char, 128>;
        MeshNameType MeshName{};

        //---------------------------------------
        // Anything Else Other than Assimp Models
        //---------------------------------------
        ECVec3 color{ 1.0f, 1.0f, 1.0f };
        ECVec2 textureIdx{ 0.0, 0.0 };
        bool isQuad = false;
        glm::mat4 modelNDC_xform{ 0 };
        std::string modelRef{ "Cube" };
        std::string shaderRef{ "shader3DShdrpgm" };
        float transparency{ 1.0f };

        enum class MapType
        {
            MT_REFLECT ,
            MT_REFRACT 
        };

        bool ENV_MAP = false;
        MapType ENV_TYPE = MapType::MT_REFLECT;
    };
}
#endif /* MESHCOMPONENT_H */