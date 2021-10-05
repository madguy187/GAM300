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
        std::array<char, 128> MeshName;

        //---------------------------------------
        // Anything Else Other than Assimp Models
        //---------------------------------------
        ECVec3 color{ 1.0f, 1.0f, 1.0f };
        ECVec2 textureIdx{ 0.0, 0.0 };
        bool isQuad = false;
        glm::mat4 modelNDC_xform{ 0 };
        std::string modelRef;
        std::string shaderRef;
        float transparency{ 1.0f };

        // For Environmental Mapping
        bool ENV_MAP = true;
    };
}
#endif /* MESHCOMPONENT_H */