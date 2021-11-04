#include "pch.h"
#include "Graphics/Lighting/LightManager.h"
#include "LightManager.h"

namespace Eclipse
{
    void LightManager::Init()
    {
        // LightManager Init =============================
        CreateAttenuationLevels();
        _DirectionalLights.FirstGlobalLight();
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

    void LightManager::DrawPointLights(unsigned int EntityId, PointLightComponent* in, FrameBufferMode Mode, unsigned int indexID, GLenum mode)
    {
        _allpointlights.Draw(EntityId, in, Mode, indexID, mode);
    }

    void LightManager::DrawDirectionalLight(unsigned int EntityId, DirectionalLightComponent* in, FrameBufferMode Mode, unsigned int indexID, GLenum mode)
    {
        _DirectionalLights.Draw(EntityId, in, Mode, indexID, mode);
    }

    void LightManager::DrawSpotLight(unsigned int EntityId, SpotLightComponent* in, FrameBufferMode Mode, unsigned int indexID, GLenum mode)
    {
        _allspotlights.Draw(EntityId, in, Mode, indexID, mode);
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

    void LightManager::GlobalUniformsUpdate()
    {
        //auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
        //shdrpgm.Use();
        //GLint NumberOfPointLights = shdrpgm.GetLocation("NumberOfPointLights");
        //GLint NumberOfSpotLights = shdrpgm.GetLocation("NumberOfSpotLights");
        //GLint NumberOfDirectionalLights = shdrpgm.GetLocation("NumberOfDirectionalLights");
        //GLCall(glUniform1i(NumberOfPointLights, _allpointlights.GetNumberOfPointLights()));
        //GLCall(glUniform1i(NumberOfSpotLights, _allspotlights.GetNumberOfSpotLights()));
        //GLCall(glUniform1i(NumberOfDirectionalLights, _DirectionalLights.DirectionalLightcounter));
        //shdrpgm.UnUse();

        auto shdrpgm2 = Graphics::shaderpgms["PBRShader"];
        shdrpgm2.Use();
        GLint NumberOfPointLightsforPBR = shdrpgm2.GetLocation("NumberOfPointLights");
        GLint NumberOfSpotLightsforPBR = shdrpgm2.GetLocation("NumberOfSpotLights");
        GLint Directional = shdrpgm2.GetLocation("Directional");

        GLCall(glUniform1i(NumberOfPointLightsforPBR, _allpointlights.GetNumberOfPointLights()));
        GLCall(glUniform1i(NumberOfSpotLightsforPBR, _allspotlights.GetNumberOfSpotLights()));
        GLCall(glUniform1i(Directional, _DirectionalLights.DirectionalLightcounter));
        shdrpgm2.UnUse();
    }

    void LightManager::SpotLightUpdate(Entity ID)
    {
        float UPWARD = 0.0f;
        float RightVec = 0.0f;
        glm::vec2 NewDir = { 0,0 };
        int Negative = 0;

        auto& SpotLight_ = engine->world.GetComponent<SpotLightComponent>(ID);
        auto& transCom = engine->world.GetComponent<TransformComponent>(ID);

        float GetMagnitudeOfDirection = SpotLight_.direction.ConvertToGlmVec3Type().length(); 
        glm::vec3 dir = glm::normalize(SpotLight_.direction.ConvertToGlmVec3Type());

        if (transCom.rotation.getX() > 0)
        {
            Negative = 1;
        }
        else
        {
            Negative = -1;
        }

        //Quad 2
        if (transCom.rotation.getX() > 90 && transCom.rotation.getX() < 180 || transCom.rotation.getX() < -90 && transCom.rotation.getX() > -180)
        {
            UPWARD = tanf(glm::radians(abs(transCom.rotation.getX()))) * -1 * Negative;
            NewDir = glm::vec2(1, 0) + +glm::vec2(0, UPWARD);
        }

        //quad 3
        if (transCom.rotation.getX() > 181 && transCom.rotation.getX() < 270 || transCom.rotation.getX() < -180 && transCom.rotation.getX() > -270)
        {
            UPWARD = tanf(glm::radians(abs(transCom.rotation.getX()))) * -1 * Negative;
            NewDir = glm::vec2(1, 0) + glm::vec2(0, UPWARD);
        }

        //quad4
        if (transCom.rotation.getX() > 270 && transCom.rotation.getX() < 360 || transCom.rotation.getX() < -270 && transCom.rotation.getX() > -360)
        {
            UPWARD = tanf(glm::radians(abs(transCom.rotation.getX()))) * Negative;
            NewDir = glm::vec2(-1, 0) + glm::vec2(0, UPWARD);
        }

        //quad1
        if (transCom.rotation.getX() > 0 && transCom.rotation.getX() < 90 || transCom.rotation.getX() < 0 && transCom.rotation.getX() > -90)
        {
            UPWARD = tanf(glm::radians(abs(transCom.rotation.getX()))) * Negative;
            NewDir = glm::vec2(-1, 0) + glm::vec2(0, UPWARD);
        }

        glm::vec2 ScaleupDir = NewDir * GetMagnitudeOfDirection;
        SpotLight_.direction.setX(0.0f);
        SpotLight_.direction.setY(ScaleupDir.y);
        SpotLight_.direction.setZ(ScaleupDir.x);

        if (transCom.rotation.getX() == 270.0f || transCom.rotation.getX() == -270.0f)
        {
            SpotLight_.direction.setX(0.0f);
            SpotLight_.direction.setY(-5.0f * Negative);
            SpotLight_.direction.setZ(0.0f);
        }

        if (transCom.rotation.getX() == 0.0f)
        {
            SpotLight_.direction.setX(0.0f);
            SpotLight_.direction.setY(0.0f);
            SpotLight_.direction.setZ(-5.0f);
        }

        if (transCom.rotation.getX() == 90.0f || transCom.rotation.getX() == -90.0f)
        {
            SpotLight_.direction.setX(0.0f);
            SpotLight_.direction.setY(5.0f * Negative);
            SpotLight_.direction.setZ(0.0f);
        }

        if (transCom.rotation.getX() == 180.0f || transCom.rotation.getX() == -180.0f)
        {
            SpotLight_.direction.setX(0.0f);
            SpotLight_.direction.setY(0.0f);
            SpotLight_.direction.setZ(5.0f);
        }

        if (transCom.rotation.getX() == 360.0f || transCom.rotation.getX() == -360.0f)
        {
            SpotLight_.direction.setX(0.0f);
            SpotLight_.direction.setY(0.0f);
            SpotLight_.direction.setZ(-5.0f);
        }
    }

    void LightManager::DestroyLight(Entity ID)
    {
        if (engine->world.CheckComponent<PointLightComponent>(ID))
        {
            _allpointlights.Destroy();
        }
        else if (engine->world.CheckComponent<SpotLightComponent>(ID) == true)
        {
            _allspotlights.Destroy();
        }
        else if (engine->world.CheckComponent<DirectionalLightComponent>(ID) == true)
        {
            _DirectionalLights.Destroy();
        }
    }
}