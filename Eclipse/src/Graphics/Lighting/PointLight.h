#pragma once
#include "ECS/ComponentManager/Components/PointLightComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
    typedef std::map<int, PointLightComponent*> PointLightContainer;

    class PointLight
    {
    private:
        inline static int counter = 0;
        InputWrapper inputhandler;
        inline static PointLightContainer _pointlights;

    public:
        PointLightContainer GetContainer();
        unsigned int GetNumberOfPointLights();
        static void CreatePointLight(unsigned int CreatedID);
        void DrawPointLights(unsigned int framebufferID);
        void Draw(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);

    private:
        void CheckUniformLoc(Shader* _shdrpgm, PointLightComponent& hi, int index, unsigned int containersize);
    };
}
