#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/InputHandler/AllInputKeyCodes.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

//Components
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "SparseSet/SparseSet.hpp"

void Eclipse::RenderSystem::Init()
{
    ENGINE_CORE_INFO("RenderSystem Init");
    engine->gGraphics.pre_render();

    engine->AssimpManager.LoadAllModels();

    // SKY
    skybox.init();
    skybox.loadTextures("src/Assets/Sky");
}

Signature Eclipse::RenderSystem::RegisterAll()
{
    Signature SystemSignature;

    SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
    SystemSignature.set(engine->world.GetComponentType<RenderComponent>(), 1);
    engine->world.RegisterSystemSignature<RenderSystem>(SystemSignature);

    return SystemSignature;
}

void Eclipse::RenderSystem::Update()
{
    // SKY =============================
    glBindFramebuffer(GL_FRAMEBUFFER, engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
    auto shdrpgm = Graphics::shaderpgms.find("Sky");
    skybox.render(shdrpgm->second);
    shdrpgm->second.UnUse();

    // RENDERCOMPONENTS & TRANSFORMCOMPONENT =============================
    for (auto const& entity : mEntities)
    {
        RenderComponent& _Sprites = engine->world.GetComponent<RenderComponent>(entity);
        engine->gGraphics.Draw(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
        engine->gGraphics.Draw(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
    }

    // CAMERA =============================
    engine->gDebugManager.DrawDebugShapes(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

    // MODELS =============================
    engine->AssimpManager.Draw(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), GL_FILL);
    engine->AssimpManager.Draw(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);
}