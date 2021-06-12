#pragma once
#include <map>
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/DirectionalLight.h"


namespace Eclipse
{
    enum class TypesOfLights
    {
        NONE,
        POINTLIGHT,
        DIRECTIONAL,
        MAXCOUNT
    };

    class Lights
    {
    private:
        PointLight _allpointlights;
        DirectionalLight _DirectionalLights;

    public:

    public:
        void init();
        void Update();
        void CreateLights(TypesOfLights in, unsigned int CreatedID);
        void DrawPointLights(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        void DrawDirectionalLight(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        PointLightContainer GetPointLightsContainer();
        DirectionalLightContainer GetDirectionalLightContainer();
    };
}