#include "pch.h"
#include "LightingSystem.h"

void Eclipse::LightingSystem::Init()
{
    ENGINE_CORE_INFO("LightingSystem Init");
}

void Eclipse::LightingSystem::Update()
{
    // Loop
    for (auto const& entity : engine->LightManager.GetPointLightsContainer())
    {
        engine->gGraphics.ShowTestWidgets(entity.second->ID, engine->gGraphics.createdID);

        engine->LightManager.DrawPointLights(entity.second,
            engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity.first, GL_FILL);

        engine->LightManager.DrawPointLights(entity.second,
            engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), entity.first, GL_LINE);
    }
}
