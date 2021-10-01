#include "pch.h"
#include "LightingSystem.h"

namespace Eclipse
{
	void LightingSystem::Init()
	{
		EDITOR_LOG_INFO("LightingSystem Init");
		engine->LightManager.init();
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
		engine->Timer.tracker.system_start = glfwGetTime();

		if (engine->LightManager.CheckApplyLighting() == true)
		{
			for (auto const& LightEntityID : mEntities)
			{
				auto& Light = engine->world.GetComponent<LightComponent>(LightEntityID);

				Light.Render = true; // engine->gCullingManager->ToRenderOrNot(LightEntityID);

				if (engine->world.CheckComponent<DirectionalLightComponent>(LightEntityID))
				{
					auto& DirectionalLight = engine->world.GetComponent<DirectionalLightComponent>(LightEntityID);

					engine->LightManager.DrawDirectionalLight(
						LightEntityID,
						&DirectionalLight, // DirectionalLightComponent
						engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::FBM_SCENE)->GetFrameBufferID(), // FrameBufferID
						DirectionalLight.Counter, // ID used to loop in shaders
						GL_FILL);
				}
				else if (engine->world.CheckComponent<SpotLightComponent>(LightEntityID))
				{
					auto& SpotLight = engine->world.GetComponent<SpotLightComponent>(LightEntityID);

					engine->LightManager.DrawSpotLight(
						LightEntityID,
						&SpotLight, // SpotLightComponent
						engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::FBM_SCENE)->GetFrameBufferID(), // FrameBuffer ID
						SpotLight.Counter, // ID used to loop in shaders
						GL_FILL);
				}
				else if (engine->world.CheckComponent<PointLightComponent>(LightEntityID))
				{
					auto& PointLight = engine->world.GetComponent<PointLightComponent>(LightEntityID);

					engine->LightManager.DrawPointLights(
						LightEntityID,
						&PointLight, // PointLightComponent
						engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::FBM_SCENE)->GetFrameBufferID(), // FrameBufferID
						PointLight.Counter, // ID used to loop in shaders
						GL_FILL);
				}
			}
		}

		engine->Timer.tracker.system_end = glfwGetTime();
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

		FrameMark
	}
}