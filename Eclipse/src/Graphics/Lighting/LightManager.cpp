#include "pch.h"
#include "Graphics/Lighting/LightManager.h"

void Eclipse::Lights::init()
{
    CreateAttenuationLevels();
    //engine->LightManager._DirectionalLights.FirstGlobalLight();
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
        _DirectionalLights.CreateDirectionalLight(CreatedID);
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

void Eclipse::Lights::CreateAttenuationLevels()
{
    // Smallest Range
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(1, AttenuationValues(1.0f, 0.7f, 1.8f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(2, AttenuationValues(1.0f, 0.35f, 0.44f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(3, AttenuationValues(1.0f, 0.22f, 0.20f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(4, AttenuationValues(1.0f, 0.14f, 0.07f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(5, AttenuationValues(1.0f, 0.09f, 0.032f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(6, AttenuationValues(1.0f, 0.07f, 0.017f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(7, AttenuationValues(1.0f, 0.045f, 0.0075f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(8, AttenuationValues(1.0f, 0.027f, 0.0028f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(9, AttenuationValues(1.0f, 0.022f, 0.0019f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(10, AttenuationValues(1.0f, 0.014f, 0.0007f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(11, AttenuationValues(1.0f, 0.007f, 0.0002f)));
    AttenuationLevels.push_back(std::pair<unsigned int, AttenuationValues>(12, AttenuationValues(1.0f, 0.0014f, 0.00000f)));
    // Largest Range

    std::cout << AttenuationLevels.size() << " Attentuation Levels Created! " << std::endl;
}

void Eclipse::Lights::SetAttenuation(PointLightComponent& in, unsigned int Level)
{
    in.constant = AttenuationLevels[Level].second.Constant_;
    in.linear = AttenuationLevels[Level].second.Linear_;
    in.quadratic = AttenuationLevels[Level].second.Quadratic_;
}

PointLightContainer Eclipse::Lights::GetPointLightsContainer()
{
    return _allpointlights.GetContainer();
}
