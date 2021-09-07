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

        // Create AABB Boxes =============================
        engine->GraphicsManager.AllAABBs.Init();
    }

    Signature RenderSystem::RegisterAll()
    {
        Signature SystemSignature;

        SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
        SystemSignature.set(engine->world.GetComponentType<RenderComponent>(), 1);
        SystemSignature.set(engine->world.GetComponentType<MaterialComponent>(), 1);
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
            // SKY Render Start =============================
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
            engine->AssimpManager.MeshDraw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID(), GL_FILL, &engine->GraphicsManager.AllAABBs, CameraComponent::CameraType::Editor_Camera);

            engine->MaterialManager.DoNotUpdateStencil();
            engine->AssimpManager.MeshDraw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::GAMEVIEW)->GetFrameBufferID(), GL_FILL, &box, CameraComponent::CameraType::Game_Camera);
            engine->AssimpManager.MeshDraw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_TOP)->GetFrameBufferID(), GL_FILL, &box, CameraComponent::CameraType::TopView_Camera);
            engine->AssimpManager.MeshDraw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_BOTTOM)->GetFrameBufferID(), GL_FILL, &box, CameraComponent::CameraType::BottomView_Camera);
            engine->AssimpManager.MeshDraw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_LEFT)->GetFrameBufferID(), GL_FILL, &box, CameraComponent::CameraType::RightView_camera);
            engine->AssimpManager.MeshDraw(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SWITCHINGVIEWS_RIGHT)->GetFrameBufferID(), GL_FILL, &box, CameraComponent::CameraType::LeftView_Camera);
            // MODELS Render  End ===============================

            // Debug Boxes Draw Start =============================
            engine->MaterialManager.DoNotUpdateStencil();
            engine->GraphicsManager.DrawDebugBoxes();
            // Debug Boxes Draw End ===============================
        }

        timer.tracker.system_end = glfwGetTime();
        timer.ContainerAddTime(timer.tracker);
    }
}