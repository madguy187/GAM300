#pragma once
#include "ECS/ComponentManager/Components/SpotLightComponent.h"

namespace Eclipse
{
    typedef std::unordered_map<int, SpotLightComponent*> SpotLightContainer;
    using SLIT = std::unordered_map<int, SpotLightComponent*>::iterator;

    class SpotLight
    {
    private:
        inline static int counter = 0;
        InputWrapper inputhandler;
        inline static SpotLightContainer _spotlights;

    public:
        SpotLightContainer GetContainer();
        unsigned int GetNumberOfSpotLights();
        static void CreateSpotLight(unsigned int CreatedID);
        static bool DeleteSpotLight(unsigned int EntityID);
        void DrawSpotLights(unsigned int framebufferID);
        void Draw(SpotLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);

    private:
        void CheckUniformLoc(Shader* _shdrpgm, SpotLightComponent& hi, int index, unsigned int containersize);
    };
}
