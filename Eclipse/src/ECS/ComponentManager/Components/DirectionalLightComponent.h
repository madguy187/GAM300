#ifndef DIRECTIONALIGHTCOMPONENT_H
#define DIRECTIONALIGHTCOMPONENT_H
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct DirectionalLightComponent
    {
        META_DATA(DirectionalLightComponent);
        // To Loop In Container in Shaders
        unsigned int Counter = 0;

        // Properties
        bool visible = false;
        bool AffectsWorld = true;
        bool EnableBlinnPhong = true;
        ECVec3 lightColor{ 1.0f,1.0f,1.0f };
        ECVec3 Direction{ -10.0f,-10.0f,-10.0f };
        ECVec3 ambient{ 0.2f , 0.2f, 0.2f };
        ECVec3 diffuse{ 0.4f,0.4f,0.4f };
        ECVec3 specular{ 0.5f,0.5f,0.5f };
        ECVec4 Color{ 1.0f,1.0f,1.0f,1 };

        // Overall 
        ECVec3 AmbientSettings{ 0.05f };
    };
}
#endif /* DIRECTIONALIGHTCOMPONENT_H */