#pragma once
#include "ECS/ComponentManager/Components/PointLightComponent.h"

namespace Eclipse
{
    typedef std::map<int, PointLightComponent*> PointLightContainer;
    using PLIT = std::map<int, PointLightComponent*>::iterator;

    class PointLight
    {
    private:
        inline static Eclipse::PointLightContainer _pointlights;

    public:
        inline static int counter = 0;
        InputWrapper inputhandler;
        static void CreatePointLight(unsigned int CreatedID);
        void DrawPointLights(unsigned int framebufferID);
        void CheckUniformLoc(Graphics::shaderIt _shdrpgm, PointLightComponent& hi, int index, unsigned int containersize);
        void Draw(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID);
    };
}
