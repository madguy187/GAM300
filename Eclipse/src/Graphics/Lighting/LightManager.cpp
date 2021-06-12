#include "pch.h"
#include "Graphics/Lighting/LightManager.h"

void Eclipse::Lights::init()
{
    //auto& World_ = engine->world;
    //Entity EntityID = World_.CreateEntity();
    //CreateLights(TypesOfLights::DIRECTIONAL,EntityID);
}

void Eclipse::Lights::Update()
{

}

void Eclipse::Lights::CreateLights(TypesOfLights in, unsigned int CreatedID)
{
    switch (in)
    {
    case TypesOfLights::POINTLIGHT:
    {
        _allpointlights.CreatePointLight(CreatedID);
    }
    break;

    case TypesOfLights::DIRECTIONAL:
    {
        // ill use shader first
        //_DirectionalLights.CreateDirectionalLight(CreatedID);
    }
    break;
    }
}

void Eclipse::Lights::DrawPointLights(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    _allpointlights.Draw(in, framebufferID, indexID, mode);
}

void Eclipse::Lights::DrawDirectionalLight(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    _DirectionalLights.Draw(in, framebufferID, indexID, mode);
}

DirectionalLightContainer Eclipse::Lights::GetDirectionalLightContainer()
{
    return _DirectionalLights.GetContainer();
}

PointLightContainer Eclipse::Lights::GetPointLightsContainer()
{
    return _allpointlights.GetContainer();
}
