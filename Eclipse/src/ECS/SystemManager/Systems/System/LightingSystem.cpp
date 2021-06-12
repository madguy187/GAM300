#include "pch.h"
#include "LightingSystem.h"

void Eclipse::LightingSystem::Init()
{
    ENGINE_CORE_INFO("LightingSystem Init");
    engine->LightManager.init();
}

void Eclipse::LightingSystem::Update()
{
    // Loop
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
}