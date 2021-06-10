#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/InputHandler/AllInputKeyCodes.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

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
         - To be removed

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
   /* ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();*/

    engine->gGraphics.UpdateFrameBuffer();

    // Loop
    for (auto const& entity : mEntities)
    {
        Sprite& _Sprites = engine->world.GetComponent<Sprite>(entity);

        engine->gGraphics.ShowTestWidgets(entity, engine->gGraphics.createdID);
        engine->gGraphics.DrawBuffers(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
        engine->gGraphics.DrawBuffers(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), &_Sprites, GL_LINE);
    }

    engine->gDebugManager.DrawDebugShapes(engine->gGraphics.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

    engine->gGraphics.FrameBufferDraw();
    /*ImGui::Render();*/
    /*engine->gGraphics.ImguiRender();*/
    //engine->gGraphics.post_render();
}