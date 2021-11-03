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

        engine->gEngineCompiler = std::make_unique<EngineCompiler>();
        engine->gPBRManager = std::make_unique<PBRManager>();
        engine->gCullingManager = std::make_unique<CullingManager>();

        // Outlining Preparation ============================= 
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // Load All Compilers =============================
        engine->gEngineCompiler->Init();

        // Create SKY =============================
        engine->GraphicsManager.CreateSky("src/Assets/Sky");

        // DebugManagerRender
        engine->gDebugDrawManager->Init();

        engine->gPBRManager->gMaterialEditorSettings->CreateModel();
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
            for (auto const& entityID : RenderablesVsFrustrum)
            {
                //If No Mesh Component, Do not Continue
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

                // After hot-realoding , we check if he still exists or not
                if (engine->AssimpManager.CheckGeometryExist(Mesh))
                {
                    if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
                    {
                        engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                        engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_SCENE, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SCENE),
                            CameraComponent::CameraType::Editor_Camera);

                        // See Normal Vectors
                        engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                        engine->AssimpManager.DebugNormals(Mesh, entityID, FrameBufferMode::FBM_SCENE, CameraComponent::CameraType::Editor_Camera);
                    }

                    if (engine->editorManager->GetEditorWindow<eGameViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_GAME, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME),
                            CameraComponent::CameraType::Game_Camera);
                    }

                    // Top View Port
                    if (engine->editorManager->GetEditorWindow<TopSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_TOP, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_TOP),
                            CameraComponent::CameraType::TopView_Camera);
                    }

                    // Bottom View port
                    if (engine->editorManager->GetEditorWindow<BottomSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_BOTTOM, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_BOTTOM),
                            CameraComponent::CameraType::BottomView_Camera);
                    }

                    // Left View Port
                    if (engine->editorManager->GetEditorWindow<LeftSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_LEFT, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_LEFT),
                            CameraComponent::CameraType::LeftView_Camera);
                    }

                    // Right ViewPort
                    if (engine->editorManager->GetEditorWindow<RightSwitchViewWindow>()->IsVisible)
                    {
                        engine->MaterialManager.DoNotUpdateStencil();
                        engine->AssimpManager.MeshDraw(Mesh, entityID, FrameBufferMode::FBM_RIGHT, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_RIGHT),
                            CameraComponent::CameraType::RightView_camera);
                    }

                    engine->MaterialManager.Highlight3DModels(entityID, FrameBufferMode::FBM_SCENE);
                }
            }

            engine->AssimpManager.MeshEditor_.Render();

            if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
            {
                engine->MaterialManager.DoNotUpdateStencil();
                engine->gDebugManager.DrawDebugShapes(FrameBufferMode::FBM_SCENE);
            }
            engine->MaterialManager.StencilBufferClear();
        }

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

        engine->gDebugDrawManager->Render();

        FrameMark
    }
}