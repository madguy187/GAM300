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

void Eclipse::Lights::DrawLights(unsigned int framebufferID)
{
    // Draw All Point Lights
    _allpointlights.DrawPointLights(framebufferID);

}
