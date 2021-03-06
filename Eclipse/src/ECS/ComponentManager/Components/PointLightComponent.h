#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H

#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct PointLightComponent
    {
        META_DATA(PointLightComponent);
        unsigned int Counter = 0; // To iterate in Shaders

        ECVec3 ambient{ 0.2f , 0.2f, 0.2f };
        ECVec3 diffuse{ 0.5f,0.5f,0.5f };
        ECVec3 specular{ 0.1f,0.1f,0.1f };
        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec4 Color{ 1.0f,1.0f,1.0f,1.0f };

        float IntensityStrength = 0.5f;
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        float radius = 0.0f;
        int AttenuationLevel = 2;

        bool EnableBlinnPhong = true;
        bool hasTexture = false;
        bool visible = true;
        bool AffectsWorld = true;

        ECVec4 RGBColor{ 1.0f,1.0f,1.0f,1.0f };
    };
}
#endif /* POINTLIGHTCOMPONENT_H */
