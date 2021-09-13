#pragma once
#include "ECS/ComponentManager/Components/SpotLightComponent.h"

namespace Eclipse
{
    typedef std::unordered_map<int, SpotLightComponent*> SpotLightContainer;
    using SLIT = std::unordered_map<int, SpotLightComponent*>::iterator;

    class SpotLight
    {
    private:
        inline static unsigned int SpotLightCounter = 0;

    public:
        unsigned int GetNumberOfSpotLights();
        static void CreateSpotLight(unsigned int CreatedID);
        void DrawSpotLights(unsigned int framebufferID);
        void Draw(SpotLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
    private:
        void CheckUniformLoc(Shader* _shdrpgm, SpotLightComponent& hi, int index, unsigned int containersize);

    private:
        inline static SpotLightContainer _spotlights;
        SpotLightContainer GetContainer();
        static bool DeleteSpotLight(unsigned int EntityID);
        bool InsertSpotLightLight(SpotLightComponent& in);
        void ClearContainer();

    };
}
