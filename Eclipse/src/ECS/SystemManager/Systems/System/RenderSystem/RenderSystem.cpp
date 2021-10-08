#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

//Components
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/MeshComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "ECS/ComponentManager/Components/AABBComponent.h"

// Views
#include "Editor/Windows/SwitchViews/TopSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/BottomSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/LeftSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/RightSwitchViewWindow.h"

namespace Eclipse
{
    void RenderSystem::Init()
    {
        // Register Threads
        engine->GraphicsManager.RegisterThreads();

        // Compilers ===========================
        engine->gEngineCompiler = std::make_unique<EngineCompiler>();

        // Outlining Preparation ============================= 
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // Graphics Init =============================
        EDITOR_LOG_INFO("RenderSystem Init");
        //engine->GraphicsManager.DebugPrintFrameBuffers();

        // Load All Compilers =============================
        engine->gEngineCompiler->Init();

        // Create SKY =============================
        engine->GraphicsManager.CreateSky("src/Assets/Sky");

        // Create AABB Boxes =============================
        engine->GraphicsManager.AllAABBs.Init();

        // CUlling =============================
        engine->gCullingManager = std::make_unique<CullingManager>();
    }

    void RenderSystem::Update()
    {
        ZoneScopedN("Render System")
            engine->Timer.SetName({ SystemName::RENDER });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        engine->GraphicsManager.UploadGlobalUniforms();

        if (engine->GraphicsManager.CheckRender == true)
        {
            // Estiamtion which models are in our frustrum
            auto RenderablesVsFrustrum = engine->gCullingManager->ReturnContacted();

            /*************************************************************************
              Render Without Stencer
              Render Sky to Sceneview
            *************************************************************************/
            // engine->MaterialManager.DoNotUpdateStencil();
            // engine->GraphicsManager.RenderSky(FrameBufferMode::FBM_SCENE);

             // Basic Primitives Render Start =============================
            for (auto const& entityID : RenderablesVsFrustrum)
            {
                // If No Mesh Component , Do not Continue
                if (!engine->world.CheckComponent<MeshComponent>(entityID))
                {
                    continue;
                }

                // If CUlled off , dont render
                if (engine->gCullingManager->ToRenderOrNot(entityID) == false)
                {
                    continue;
                }

                MeshComponent& Mesh = engine->world.GetComponent<MeshComponent>(entityID);
                engine->MaterialManager.UpdateShininess(entityID);

                // Basic Primitives
                if (!engine->world.CheckComponent<ModelComponent>(entityID))
                {
                    engine->GraphicsManager.CheckTexture(entityID);

                    /*************************************************************************
                      Render Primitives to SceneView
                    *************************************************************************/
                    engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                    engine->GraphicsManager.Draw(FrameBufferMode::FBM_SCENE, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::Editor_Camera);

                    /*************************************************************************
                      Render Without Stencer , Render Primitivies to GameView
                    *************************************************************************/
                    //engine->MaterialManager.DoNotUpdateStencil();
                    //engine->GraphicsManager.Draw(FrameBufferMode::FBM_GAME, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::Game_Camera);
                    //
                    //engine->MaterialManager.DoNotUpdateStencil();
                    //engine->GraphicsManager.Draw(FrameBufferMode::FBM_LEFT, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::LeftView_Camera);
                    //
                    //engine->MaterialManager.DoNotUpdateStencil();
                    //engine->GraphicsManager.Draw(FrameBufferMode::FBM_RIGHT, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::RightView_camera);

                     // Highlight
                    engine->MaterialManager.HighlightBasicPrimitives(entityID, FrameBufferMode::FBM_SCENE);
                }

                //else
                //{
                //    // After hot-realoding , we check if he still exists or not
                //    if (engine->AssimpManager.CheckGeometryExist(Mesh))
                //    {
                //        /*************************************************************************
                //          Render Models to SceneView
                //        *************************************************************************/
                //        engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                //        engine->AssimpManager.MeshDraw(Mesh, entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_SCENE), engine->GraphicsManager.GetRenderMode(FrameBufferMode::FBM_SCENE),
                //            &engine->GraphicsManager.AllAABBs, CameraComponent::CameraType::Editor_Camera);

                //        // See Normal Vectors
                //        engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                //        engine->AssimpManager.DebugNormals(Mesh, entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_SCENE), CameraComponent::CameraType::Editor_Camera);

                //        /*************************************************************************
                //          Render Without Stencer , Render Models to GameView
                //        *************************************************************************/
                //        engine->MaterialManager.DoNotUpdateStencil();
                //        engine->AssimpManager.MeshDraw(Mesh, entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_GAME), engine->GraphicsManager.GetRenderMode(FrameBufferMode::FBM_GAME),
                //            &box, CameraComponent::CameraType::Game_Camera);

                //        // Top View Port
                //        if (engine->editorManager->GetEditorWindow<TopSwitchViewWindow>()->IsVisible)
                //        {
                //            engine->MaterialManager.DoNotUpdateStencil();
                //            engine->AssimpManager.MeshDraw(Mesh, entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_TOP), engine->GraphicsManager.GetRenderMode(FrameBufferMode::FBM_TOP),
                //                &box, CameraComponent::CameraType::TopView_Camera);
                //        }

                //        // Bottom View port
                //        if (engine->editorManager->GetEditorWindow<BottomSwitchViewWindow>()->IsVisible)
                //        {
                //            engine->MaterialManager.DoNotUpdateStencil();
                //            engine->AssimpManager.MeshDraw(Mesh, entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_BOTTOM), engine->GraphicsManager.GetRenderMode(FrameBufferMode::FBM_BOTTOM),
                //                &box, CameraComponent::CameraType::BottomView_Camera);
                //        }

                //        // Left View Port
                //        if (engine->editorManager->GetEditorWindow<LeftSwitchViewWindow>()->IsVisible)
                //        {
                //            engine->MaterialManager.DoNotUpdateStencil();
                //            engine->AssimpManager.MeshDraw(Mesh, entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_LEFT), engine->GraphicsManager.GetRenderMode(FrameBufferMode::FBM_LEFT),
                //                &box, CameraComponent::CameraType::LeftView_Camera);
                //        }

                //        // Right ViewPort
                //        if (engine->editorManager->GetEditorWindow<RightSwitchViewWindow>()->IsVisible)
                //        {
                //            engine->MaterialManager.DoNotUpdateStencil();
                //            engine->AssimpManager.MeshDraw(Mesh, entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_RIGHT), engine->GraphicsManager.GetRenderMode(FrameBufferMode::FBM_RIGHT),
                //                &box, CameraComponent::CameraType::RightView_camera);
                //        }

                //        engine->MaterialManager.Highlight3DModels(entityID, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_SCENE));
                //    }
                //}
            }

            /*************************************************************************
              Render Without Stencer , Frustrum to Scene View
            *************************************************************************/
            //engine->MaterialManager.DoNotUpdateStencil();
            //engine->gDebugManager.DrawDebugShapes(engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::FBM_SCENE)->GetFrameBufferID());

            engine->MaterialManager.StencilBufferClear();
        }

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

        FrameMark
    }
}