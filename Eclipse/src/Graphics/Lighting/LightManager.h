#pragma once
#include <map>
#include "Graphics/Lighting/PointLight.h"


namespace Eclipse
{
    enum class TypesOfLights
    {
        NONE,
        POINTLIGHT,
        MAXCOUNT
    };

    class Lights
    {
    private:
        PointLight _allpointlights;

    public:

    public:
        void Update();
        void CreateLights(TypesOfLights in, unsigned int CreatedID);
        void DrawPointLights(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        PointLightContainer GetPointLightsContainer();
    };
}