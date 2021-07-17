#pragma once
#include "ECS/ComponentManager/Components/PointLightComponent.h"

namespace Eclipse
{
    typedef std::map<int, PointLightComponent*> PointLightContainer;

    using PLIT = std::map<int, PointLightComponent*>::iterator;
    using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
    using shaderIt = std::map<std::string, Shader>::iterator;

    class PointLight
    {
    private:
        inline static int counter = 0;
        InputWrapper inputhandler;
        inline static Eclipse::PointLightContainer _pointlights;

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
