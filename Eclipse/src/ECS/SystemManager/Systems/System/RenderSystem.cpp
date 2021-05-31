#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/InputHandler/AllInputKeyCodes.h"

//Components
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"

/*************************************************************************
  RenderSystem

  ! Note all imgui stuffs will be taken out

  Init   : PreRender Init
         - Load Files
         - Clear View

  Update : Update Loop
         - Update FrameBuffers
         - Render
         - Render on FrameBuffer
         - Post Render

  RegisterAll : Set Signatures for this system

*************************************************************************/

void Eclipse::RenderSystem::Init()
{
    engine->gGraphics.pre_render();
}

Signature Eclipse::RenderSystem::RegisterAll()
{
    Signature SystemSignature;

    SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
    SystemSignature.set(engine->world.GetComponentType<Sprite>(), 1);
    engine->world.RegisterSystemSignature<RenderSystem>(SystemSignature);

    return SystemSignature;
}

void Eclipse::RenderSystem::Update()
{
    // To be Removed
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // To be removed

    engine->gGraphics.UpdateFrameBuffer();

    // Loop
    for (auto const& entity : mEntities)
    {
        Sprite& _Sprites = engine->world.GetComponent<Sprite>(entity);

        engine->gGraphics.ShowTestWidgets(entity, engine->gGraphics.createdID);
        engine->gGraphics.DrawBuffers(engine->gGraphics.mRenderContext.m_frameBuffer->GetFrameBufferID(), &_Sprites, GL_FILL);
        engine->gGraphics.DrawBuffers(engine->gGraphics.mRenderContext.n_frameBuffer->GetFrameBufferID(), &_Sprites, GL_LINE);
    }

    engine->gGraphics.draw();
    ImGui::Render();

    engine->gGraphics.ImguiRender();
    engine->gGraphics.post_render();
}