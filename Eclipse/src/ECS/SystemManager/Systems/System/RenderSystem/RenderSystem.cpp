#include "pch.h"
#include "RenderSystem.h"

// Views
#include "Editor/Windows/SwitchViews/TopSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/BottomSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/LeftSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/RightSwitchViewWindow.h"
#include "Editor/Windows/GameView/GameView.h"
#include "Editor/Windows/Scene/SceneView.h"
#include "Editor/Windows/MeshEditor/MeshEditor.h"

#include "ECS/SystemManager/Systems/System/MaterialSystem/MaterialSystem.h"

namespace Eclipse
{
    void RenderSystem::Init()
    {
        // Register Threads
        engine->GraphicsManager.RegisterThreads();

        engine->gPBRManager = std::make_unique<PBRManager>();

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

        MaterialSystem::Init();
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
            const auto& RenderablesVsFrustrum = engine->gCullingManager->ReturnContacted();

            /*************************************************************************
              Render Without Stencer
              Render Sky to Sceneview
            *************************************************************************/
            engine->MaterialManager.DoNotUpdateStencil();
            engine->GraphicsManager.RenderSky(FrameBufferMode::FBM_SCENE);

            // Basic Primitives Render Start =============================
            for (auto const& entityID : mEntities)
            {
                // If No Mesh Component , Do not Continue
                if (!engine->world.CheckComponent<MeshComponent>(entityID))
                {
                    continue;
                }

                // If it is a base prefab, dont render
                if (engine->world.CheckComponent<PrefabComponent>(entityID))
                {
                    auto& prefab = engine->world.GetComponent<PrefabComponent>(entityID);

                    if (!prefab.IsInstance)
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

                    if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
                    {
                        engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                        engine->GraphicsManager.Draw(FrameBufferMode::FBM_SCENE, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::Editor_Camera);
                    }

                    if (engine->editorManager->GetEditorWindow<eGameViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->GraphicsManager.Draw(FrameBufferMode::FBM_GAME, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::Game_Camera);
                    }

                    if (engine->editorManager->GetEditorWindow<LeftSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->GraphicsManager.Draw(FrameBufferMode::FBM_LEFT, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::LeftView_Camera);
                    }

                    if (engine->editorManager->GetEditorWindow<RightSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->GraphicsManager.Draw(FrameBufferMode::FBM_RIGHT, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::RightView_camera);
                    }

                    // Top View Port
                    if (engine->editorManager->GetEditorWindow<TopSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->GraphicsManager.Draw(FrameBufferMode::FBM_TOP, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::TopView_Camera);
                    }

                    // Bottom View port
                    if (engine->editorManager->GetEditorWindow<BottomSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->GraphicsManager.Draw(FrameBufferMode::FBM_BOTTOM, &Mesh, GL_FILL, entityID, CameraComponent::CameraType::BottomView_Camera);
                    }

                    // Highlight
                    engine->MaterialManager.HighlightBasicPrimitives(entityID, FrameBufferMode::FBM_SCENE);
                }
                else
                {
                    // After hot-realoding , we check if he still exists or not
                    if (engine->AssimpManager.CheckGeometryExist(Mesh))
                    {
                        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
                        {
                            engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                            engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_SCENE, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SCENE),
                                &engine->GraphicsManager.AllAABBs, CameraComponent::CameraType::Editor_Camera);

                            // See Normal Vectors
                            engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                            engine->AssimpManager.DebugNormals(Mesh, entityID, FrameBufferMode::FBM_SCENE, CameraComponent::CameraType::Editor_Camera);
                        }

                        if (engine->editorManager->GetEditorWindow<eGameViewWindow>()->IsVisible)
                        {
                            engine->MaterialManager.DoNotUpdateStencil();
                            engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_GAME, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME),
                                &box, CameraComponent::CameraType::Game_Camera);
                        }

                        // Top View Port
                        if (engine->editorManager->GetEditorWindow<TopSwitchViewWindow>()->IsVisible)
                        {
                            engine->MaterialManager.DoNotUpdateStencil();
                            engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_TOP, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_TOP),
                                &box, CameraComponent::CameraType::TopView_Camera);
                        }

                        // Bottom View port
                        if (engine->editorManager->GetEditorWindow<BottomSwitchViewWindow>()->IsVisible)
                        {
                            engine->MaterialManager.DoNotUpdateStencil();
                            engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_BOTTOM, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_BOTTOM),
                                &box, CameraComponent::CameraType::BottomView_Camera);
                        }

                        // Left View Port
                        if (engine->editorManager->GetEditorWindow<LeftSwitchViewWindow>()->IsVisible)
                        {
                            engine->MaterialManager.DoNotUpdateStencil();
                            engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_LEFT, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_LEFT),
                                &box, CameraComponent::CameraType::LeftView_Camera);
                        }

                        // Right ViewPort
                        if (engine->editorManager->GetEditorWindow<RightSwitchViewWindow>()->IsVisible)
                        {
                            engine->MaterialManager.DoNotUpdateStencil();
                            engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_RIGHT, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_RIGHT),
                                &box, CameraComponent::CameraType::RightView_camera);
                        }

                        engine->MaterialManager.Highlight3DModels(entityID, FrameBufferMode::FBM_SCENE);
                    }
                }
            }

            auto* meshEditor = engine->editorManager->GetEditorWindow<MeshEditorWindow>();

            if (meshEditor->IsVisible)
            {
                Entity meshID = meshEditor->GetMeshID();
                auto& mewMesh = engine->world.GetComponent<MeshComponent>(meshID);
                engine->MaterialManager.UpdateShininess(meshID);

                if (!engine->world.CheckComponent<ModelComponent>(meshID))
                {
                    engine->MaterialManager.DoNotUpdateStencil();
                    engine->GraphicsManager.Draw(FrameBufferMode::FBM_MESHEDITOR, &mewMesh, GL_FILL, meshID,
                        CameraComponent::CameraType::MeshEditor_Camera);
                }
                else
                {
                    engine->MaterialManager.DoNotUpdateStencil();
                    engine->AssimpManager.MeshEditorDraw(engine->world, mewMesh, meshID,
                        FrameBufferMode::FBM_MESHEDITOR, CameraComponent::CameraType::MeshEditor_Camera);
                }
            }

            if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
            {
                engine->MaterialManager.DoNotUpdateStencil();
                engine->gDebugManager.DrawDebugShapes(FrameBufferMode::FBM_SCENE);
            }

            engine->MaterialManager.StencilBufferClear();
        }

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

        FrameMark
    }
}