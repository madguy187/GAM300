#include "pch.h"
#include "Graphics/Lighting/LightManager.h"

void Eclipse::Lights::init()
{
    auto& World_ = engine->world;
    Entity EntityID = World_.CreateEntity();
    CreateLights(TypesOfLights::POINTLIGHT,3);
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
        _allspotlights.CreateSpotLight(CreatedID);
        //_allpointlights.CreatePointLight(CreatedID);
    }
    break;

    case TypesOfLights::DIRECTIONAL:
    {
        // ill use shader first
        //_DirectionalLights.CreateDirectionalLight(CreatedID);
    }
    break;

    case TypesOfLights::SPOTLIGHT:
    {
        _allspotlights.CreateSpotLight(CreatedID);
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

void Eclipse::Lights::DrawSpotLight(SpotLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    _allspotlights.Draw(in, framebufferID, indexID, mode);
}

DirectionalLightContainer Eclipse::Lights::GetDirectionalLightContainer()
{
    return _DirectionalLights.GetContainer();
}

SpotLightContainer Eclipse::Lights::GetSpotLightsContainer()
{
    return _allspotlights.GetContainer();
}

PointLightContainer Eclipse::Lights::GetPointLightsContainer()
{
    return _allpointlights.GetContainer();
}
