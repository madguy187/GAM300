#include "pch.h"
#include "Graphics/Lighting/LightManager.h"

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

    default:
    {

    }
    break;
    }
}

void Eclipse::Lights::DrawPointLights(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
{
    _allpointlights.Draw(in, framebufferID, indexID, mode);
}

PointLightContainer Eclipse::Lights::GetPointLightsContainer()
{
    return _allpointlights.GetContainer();
}
