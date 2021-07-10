#include "pch.h"
#include "LightingSystem.h"

void Eclipse::LightingSystem::Init()
{
    EDITOR_LOG_INFO("LightingSystem Init");
    engine->LightManager.init();
}

void Eclipse::LightingSystem::Update()
{
    ProfilerWindow timer;
    timer.SetName({ SystemName::LIGHTING });
    timer.tracker.system_start = glfwGetTime();
	
    // Directional Light ; By right should only have 1
    for (auto const& entity : engine->LightManager.GetDirectionalLightContainer())
    {
        engine->LightManager.DrawDirectionalLight(entity.second,
            engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity.first, GL_FILL);
    }

    // PointLights Draw
    for (auto const& entity : engine->LightManager.GetPointLightsContainer())
    {
        engine->LightManager.DrawPointLights(entity.second,
            engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity.first, GL_FILL);
    }

    // SpotLight Draw
    for (auto const& entity : engine->LightManager.GetSpotLightsContainer())
    {
        engine->LightManager.DrawSpotLight(entity.second,
            engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity.first, GL_FILL);
    }

    timer.tracker.system_end = glfwGetTime();

    timer.ContainerAddTime(timer.tracker);
}
