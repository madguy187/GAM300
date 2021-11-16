#include "pch.h"
#include "LightingSystem.h"

namespace Eclipse
{
    void LightingSystem::Init()
    {
        engine->LightManager.Init();

       // engine->LightManager.Hand = engine->world.CreateEntity();
       // engine->world.AddComponent(engine->LightManager.Hand, TransformComponent{});
       // engine->world.AddComponent(engine->LightManager.Hand, EntityComponent{ EntityType::ENT_LIGHT_SPOT });
       // engine->LightManager.CreateLights(TypesOfLights::SPOTLIGHT, engine->LightManager.Hand);
    }

    /*************************************************************************
      Lighting System

      0 : Directional Light
          Take Note can only have 1
          Counter here is not your Entity ID !!

      0 : Point Light
          Counter here is not your Entity ID !!

      0 : Spot Light
          Counter here is not your Entity ID !!
    *************************************************************************/
    void LightingSystem::Update()
    {
        ZoneScopedN("Lighting System")
            engine->Timer.SetName({ SystemName::LIGHTING });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        engine->LightManager.GlobalUniformsUpdate();

        if (engine->LightManager.CheckApplyLighting() == true)
        {
            for (auto const& LightEntityID : mEntities)
            {
                if (engine->world.CheckComponent<DirectionalLightComponent>(LightEntityID))
                {
                    auto& DirectionalLight = engine->world.GetComponent<DirectionalLightComponent>(LightEntityID);

                    engine->LightManager.DrawDirectionalLight(LightEntityID, &DirectionalLight, FrameBufferMode::FBM_SCENE, DirectionalLight.Counter, GL_FILL);
                }
                else if (engine->world.CheckComponent<SpotLightComponent>(LightEntityID))
                {
                    auto& SpotLight = engine->world.GetComponent<SpotLightComponent>(LightEntityID);

                    if (LightEntityID == engine->LightManager.Hand)
                    {
                        auto& SpotLightT = engine->world.GetComponent<TransformComponent>(LightEntityID);

                        auto& Cam = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
                        auto& CamT = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());

                        SpotLight.direction = ECVec3(Cam.eyeFront.x, Cam.eyeFront.y, Cam.eyeFront.z);
                        SpotLightT.position = CamT.position;
                        engine->LightManager.DrawSpotLight(LightEntityID, &SpotLight, FrameBufferMode::FBM_SCENE, SpotLight.Counter, GL_FILL);
                    }
                    else
                    {
                        engine->LightManager.DrawSpotLight(LightEntityID, &SpotLight, FrameBufferMode::FBM_SCENE, SpotLight.Counter, GL_FILL);
                    }
                }
                else if (engine->world.CheckComponent<PointLightComponent>(LightEntityID))
                {
                    auto& PointLight = engine->world.GetComponent<PointLightComponent>(LightEntityID);

                    engine->LightManager.DrawPointLights(LightEntityID, &PointLight, FrameBufferMode::FBM_SCENE, PointLight.Counter, GL_FILL);
                }
            }
        }

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
        FrameMark
    }
}