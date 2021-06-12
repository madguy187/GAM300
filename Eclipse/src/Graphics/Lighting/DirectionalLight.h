#pragma once
#include <map>
#include "ECS/ComponentManager/Components/DirectionalLightComponent.h"

namespace Eclipse
{
    typedef std::map<int, DirectionalLightComponent*> DirectionalLightContainer;
    using DLIT = std::map<int, DirectionalLightComponent*>::iterator;

    class DirectionalLight
    {
    private:
        // Directional Light Container
        inline static Eclipse::DirectionalLightContainer _DirectionalLight;

    public:
        inline static int counter = 0;
        DirectionalLightContainer GetContainer();
        static void CreateDirectionalLight(unsigned int CreatedID);
        void Draw(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        void CheckUniformLoc(Graphics::shaderIt _shdrpgm, DirectionalLightComponent& hi, int index, unsigned int containersize);
        void Draw(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID);
        void FirstGlobalLight();
    };
}