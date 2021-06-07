#pragma once
#include <map>
#include "ECS/ComponentManager/Components/PointLightComponent.h"

namespace Eclipse
{
    typedef std::map<int, PointLightComponent*> PointLightContainer;
    using PLIT = std::map<int, PointLightComponent*>::iterator;

    class Lights
    {
    public:

    private:
       inline static std::vector<PointLightComponent*> _pointlights;

    public:
        InputWrapper inputhandler;
        static void CreatePointLight();
        void DrawBuffers(unsigned int FrameBufferID, PointLightComponent* _spritecomponent, GLenum mode);
        void CheckUniformLoc(PointLightComponent& sprite, unsigned int id, unsigned int framebufferID);
        void DrawPointLights(unsigned int framebufferID);
        void SetMode(PointLightComponent& sprite);
        void CheckUniformLoc(Graphics::shaderIt _shdrpgm, PointLightComponent& _camera);
        void TESTDRAW(unsigned int ID, unsigned int framebufferID);
    };
}