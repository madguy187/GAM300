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
        // Outlining Preparation ============================= 
        glEnable(GL_STENCIL_TEST);
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

        // Enables the Depth Buffer
        //glEnable(GL_DEPTH_TEST);
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

        engine->GraphicsManager.UploadGlobalUniforms();

        if (engine->GraphicsManager.CheckRender == true)
        {
            // SKY Reder Start =============================
            engine->MaterialManager.DoNotUpdateStencil();
            engine->GraphicsManager.RenderSky(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
            // SKY Reder End ===============================


            // Basic Primitives Render Start =============================
            for (auto const& entityID : mEntities) // - using RenderComponent and TransformComponent
            {
                RenderComponent& _Sprites = engine->world.GetComponent<RenderComponent>(entityID);

                engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);

                engine->MaterialManager.DoNotUpdateStencil();
                engine->GraphicsManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), &_Sprites, GL_FILL);
            }
            // Basic Primitives Render End ==============================


            // CAMERA Render Start =============================
            engine->MaterialManager.DoNotUpdateStencil();
            engine->gDebugManager.DrawDebugShapes(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());
            // CAMERA Render End ===============================


            // MODELS Render  Start =============================
            engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL);

            engine->MaterialManager.DoNotUpdateStencil();
            engine->AssimpManager.Draw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), GL_FILL);
            // MODELS Render  End ===============================
        }

        timer.tracker.system_end = glfwGetTime();
        timer.ContainerAddTime(timer.tracker);
    }
}