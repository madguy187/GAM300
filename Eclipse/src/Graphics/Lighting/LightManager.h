#pragma once
#include <map>
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/DirectionalLight.h"
#include "Graphics/Lighting/SpotLight.h"


namespace Eclipse
{
    enum class TypesOfLights
    {
        NONE,
        POINTLIGHT,
        DIRECTIONAL,
        SPOTLIGHT,
        MAXCOUNT
    };

    class Lights
    {
    private:
        PointLight _allpointlights;
        DirectionalLight _DirectionalLights;
        SpotLight _allspotlights;

    public:

    public:
        void init();
        void Update();
        void CreateLights(TypesOfLights in, unsigned int CreatedID);

        // Draws
        void DrawPointLights(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        void DrawDirectionalLight(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        void DrawSpotLight(SpotLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);

        // Get Containers
        PointLightContainer GetPointLightsContainer();
        DirectionalLightContainer GetDirectionalLightContainer();
        SpotLightContainer GetSpotLightsContainer();
    };
}