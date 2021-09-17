#include "pch.h"
#include "RenderSystem.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

//Components
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/MeshComponent.h"
#include "AssimpModel/AssimpModel.h"

// Views
#include "Editor/Windows/SwitchViews/TopSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/BottomSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/LeftSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/RightSwitchViewWindow.h"

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
        engine->AssimpManager.Init();

        // Create SKY =============================
        engine->GraphicsManager.CreateSky("src/Assets/Sky");

        // Create AABB Boxes =============================
        engine->GraphicsManager.AllAABBs.Init();
    }

    Signature RenderSystem::RegisterAll()
    {
        Signature SystemSignature;

        SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
        SystemSignature.set(engine->world.GetComponentType<MeshComponent>(), 1);
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
            /*************************************************************************
              Render Without Stencer
              Render Sky to Sceneview
            *************************************************************************/
            engine->MaterialManager.DoNotUpdateStencil();
            engine->GraphicsManager.RenderSky(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

            // Basic Primitives Render Start =============================
            for (auto const& entityID : mEntities)
            {
                MeshComponent& Mesh = engine->world.GetComponent<MeshComponent>(entityID);

                // Basic Primitives
                if (!engine->world.CheckComponent<ModeLInforComponent>(entityID))
                {
                    /*************************************************************************
                      Render With Stencer So we prepare to Hihlight in material System
                      Render Primitives to SceneView
                    *************************************************************************/
                    engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                    engine->GraphicsManager.Draw(engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::SCENEVIEW), &Mesh, GL_FILL, entityID, CameraComponent::CameraType::Editor_Camera);

                    /*************************************************************************
                      Render Without Stencer , Render Primitivies to GameView
                    *************************************************************************/
                    engine->MaterialManager.DoNotUpdateStencil();
                    engine->GraphicsManager.Draw(
                        engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::GAMEVIEW), 
                        &Mesh, GL_FILL, entityID, 
                        CameraComponent::CameraType::Game_Camera);
                }
                else
                {
                    /*************************************************************************
                      Render With Stencer So we prepare to Hihlight in material System
                      Render Models to SceneView
                    *************************************************************************/
                    engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                    engine->AssimpManager.MeshDraw(Mesh, entityID,
                        engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::SCENEVIEW),
                        engine->GraphicsManager.GetRenderMode(Eclipse::FrameBufferMode::SCENEVIEW),
                        &engine->GraphicsManager.AllAABBs, CameraComponent::CameraType::Editor_Camera);

                    /*************************************************************************
                      Render Without Stencer , Render Models to GameView
                    *************************************************************************/
                    engine->MaterialManager.DoNotUpdateStencil();
                    engine->AssimpManager.MeshDraw(Mesh, entityID,
                        engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::GAMEVIEW),
                        engine->GraphicsManager.GetRenderMode(FrameBufferMode::GAMEVIEW),
                        &box, CameraComponent::CameraType::Game_Camera);

                    /*************************************************************************
                      Render Without Stencer
                      Render Models to Different vewports
                      0: Top View Port
                      1: Bottom View Port
                      2: Left View Port
                      3: RIght View Port
                    *************************************************************************/

                    // Top View Port
                    if (engine->editorManager->GetEditorWindow<TopSwitchViewWindow>()->IsVisible)
                    {
                        engine->AssimpManager.MeshDraw(Mesh, entityID,
                            engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::SWITCHINGVIEWS_TOP),
                            engine->GraphicsManager.GetRenderMode(FrameBufferMode::SWITCHINGVIEWS_TOP),
                            &box, CameraComponent::CameraType::TopView_Camera);
                    }

                    // Bottom View port
                    if (engine->editorManager->GetEditorWindow<BottomSwitchViewWindow>()->IsVisible)
                    {
                        engine->AssimpManager.MeshDraw(Mesh, entityID,
                            engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::SWITCHINGVIEWS_BOTTOM),
                            engine->GraphicsManager.GetRenderMode(FrameBufferMode::SWITCHINGVIEWS_BOTTOM),
                            &box, CameraComponent::CameraType::BottomView_Camera);
                    }

                    // Left View Port
                    if (engine->editorManager->GetEditorWindow<LeftSwitchViewWindow>()->IsVisible)
                    {
                        engine->AssimpManager.MeshDraw(Mesh, entityID,
                            engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::SWITCHINGVIEWS_RIGHT),
                            engine->GraphicsManager.GetRenderMode(FrameBufferMode::SWITCHINGVIEWS_RIGHT),
                            &box, CameraComponent::CameraType::RightView_camera);
                    }

                    // Right ViewPort
                    if (engine->editorManager->GetEditorWindow<RightSwitchViewWindow>()->IsVisible)
                    {
                        engine->AssimpManager.MeshDraw(Mesh, entityID,
                            engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::SWITCHINGVIEWS_LEFT),
                            engine->GraphicsManager.GetRenderMode(FrameBufferMode::SWITCHINGVIEWS_LEFT),
                            &box, CameraComponent::CameraType::LeftView_Camera);
                    }
                }
            }

            /*************************************************************************
              Render Without Stencer , Frustrum to Scene View
            *************************************************************************/
            engine->MaterialManager.DoNotUpdateStencil();
            engine->gDebugManager.DrawDebugShapes(engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

            /*************************************************************************
              Render Without Stencer , Instanced Debug Boxes to Scene View
            *************************************************************************/
            engine->MaterialManager.DoNotUpdateStencil();
            engine->GraphicsManager.DrawDebugBoxes();
        }

        timer.tracker.system_end = glfwGetTime();
        timer.ContainerAddTime(timer.tracker);
    }
}