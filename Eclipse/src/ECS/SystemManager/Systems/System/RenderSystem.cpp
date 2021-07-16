#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

//Components
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "AssimpModel/AssimpModel.h"

namespace Eclipse
{
    void RenderSystem::Init()
    {
        // Enables the Stencil Buffer
        glEnable(GL_STENCIL_TEST);
        // Sets rules for outcomes of stecil tests
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // Graphics Init =============================
        EDITOR_LOG_INFO("RenderSystem Init");
        engine->GraphicsManager.DebugPrintFrameBuffers();

        // Load All Models =============================
        engine->AssimpManager.LoadAllModels();

        // Create SKY =============================
        engine->GraphicsManager.CreateSky("src/Assets/Sky");

        // Create Grid =============================
        engine->GraphicsManager.GridManager->Init();
        engine->GraphicsManager.GridManager->DebugPrint();
    }

    Signature RenderSystem::RegisterAll()
    {
        Signature SystemSignature;

        SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
        SystemSignature.set(engine->world.GetComponentType<RenderComponent>(), 1);
        engine->world.RegisterSystemSignature<RenderSystem>(SystemSignature);

        return SystemSignature;
    }

    void RenderSystem::Update()
    {
        ProfilerWindow timer;
        timer.SetName({ SystemName::RENDER });
        timer.tracker.system_start = glfwGetTime();

        engine->GraphicsManager.UploadGammaCorrectionToShader();

        if (engine->GraphicsManager.CheckRender == true)
        {
            // SKY Reder =============================
            engine->GraphicsManager.RenderSky(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

            // (RENDERCOMPONENTS & TRANSFORMCOMPONENT) Render =============================
            for (auto const& entity : mEntities)
            {
                RenderComponent& _Sprites = engine->world.GetComponent<RenderComponent>(entity);
                engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
                engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
            }

            // CAMERA =============================
            engine->gDebugManager.DrawDebugShapes(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

            // MODELS Render=============================

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);

            engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0xFF);
            glDisable(GL_DEPTH_TEST);

            engine->AssimpManager.HighlihtDraw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            
            //engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), GL_FILL);

            // GRID Render =============================
            engine->GraphicsManager.GridManager->DrawGrid(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
        }

        timer.tracker.system_end = glfwGetTime();
        timer.ContainerAddTime(timer.tracker);
    }
}