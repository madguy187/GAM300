#include "pch.h"
#include "LightingSystem.h"

void Eclipse::LightingSystem::Init()
{
    EDITOR_LOG_INFO("LightingSystem Init");
    engine->LightManager.init();
}

void Eclipse::LightingSystem::Update()
{
    // Loop
    ProfilerWindow timer;
    timer.SetName({ SystemName::LIGHTING });
    timer.tracker.system_start = glfwGetTime();
	
    for (auto const& entity : engine->LightManager.GetDirectionalLightContainer())
    {
        engine->gGraphics.ShowTestWidgets(entity.second->ID, engine->gGraphics.createdID);

        engine->LightManager.DrawDirectionalLight(entity.second,
            engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity.first, GL_FILL);
    }

    // Loop
    for (auto const& entity : engine->LightManager.GetPointLightsContainer())
    {
        engine->gGraphics.ShowTestWidgets(entity.second->ID, engine->gGraphics.createdID);

        engine->LightManager.DrawPointLights(entity.second,
            engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity.first, GL_FILL);
    }

    timer.tracker.system_end = glfwGetTime();

    timer.ContainerAddTime(timer.tracker);
}
