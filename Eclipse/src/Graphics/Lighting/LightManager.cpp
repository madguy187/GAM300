#include "pch.h"
#include "Graphics/Lighting/LightManager.h"
#include "LightManager.h"

namespace Eclipse
{
    void LightManager::init()
    {
        // LightManager Init =============================
        ENGINE_CORE_INFO("LightManager Init");
        CreateAttenuationLevels();
        _DirectionalLights.FirstGlobalLight();
    }

    void LightManager::Update()
    {

    }

    void LightManager::CreateLights(TypesOfLights in, unsigned int CreatedID)
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

    void LightManager::SetApplyLightingFlag(bool in)
    {
        ApplyLighting = in;
    }

    bool LightManager::CheckApplyLighting()
    {
        auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
        shdrpgm->second.Use();
        GLint uniform_var_loc1 = shdrpgm->second.GetLocation("CheckApplyLighting");
        GLCall(glUniform1i(uniform_var_loc1, ApplyLighting));
        shdrpgm->second.UnUse();

        return ApplyLighting;
    }

    void LightManager::DrawPointLights(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
    {
        _allpointlights.Draw(in, framebufferID, indexID, mode);
    }

    void LightManager::DrawDirectionalLight(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
    {
        _DirectionalLights.Draw(in, framebufferID, indexID, mode);
    }

    void LightManager::DrawSpotLight(SpotLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode)
    {
        _allspotlights.Draw(in, framebufferID, indexID, mode);
    }

    void LightManager::CreateAttenuationLevels()
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

        ENGINE_CORE_INFO("Attentuation Levels Created!");
    }
}