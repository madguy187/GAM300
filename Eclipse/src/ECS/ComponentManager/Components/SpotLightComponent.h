#ifndef SPOTLIGHTCOMPONENT_H
#define SPOTLIGHTCOMPONENT_H
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct SpotLightComponent
    {
        META_DATA(SpotLightComponent);
        unsigned int Counter = 0;

        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec3 direction{ 0,0,-5 };
        ECVec3 ambient{ 0.5f , 0.5f, 0.5f };
        ECVec3 diffuse{ 0.5f,0.5f,0.5f };
        ECVec3 specular{ 0.1f,0.1f,0.1f };
        ECVec4 Color{ 1.0f,1.0f,1.0f,1.0f };

        float IntensityStrength = 0.0f;
        float radius = 0.0f;
        float cutOff = 12.5f;
        float outerCutOff = 27.5f;
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        int AttenuationLevel = 2;
        float SurroundingAttenuationLevel = 0.3f;

        bool EnableBlinnPhong = true;
        bool hasTexture = false;
        bool visible = false;
        bool AffectsWorld = true;

        ECVec4 RGBColor{ 1.0f,1.0f,1.0f,1.0f };
    };
}
#endif /* SPOTLIGHTCOMPONENT_H */