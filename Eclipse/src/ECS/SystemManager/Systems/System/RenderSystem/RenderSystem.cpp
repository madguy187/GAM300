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

        MaterialSystem::Init();
    }

    void RenderSystem::Update()
    {
        ZoneScopedN("Render System")
            engine->Timer.SetName({ SystemName::RENDER });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        // bind pre-computed IBL data
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, engine->gPBRManager->IrradianceSettings.irradianceMap);
        glActiveTexture(GL_TEXTURE20);
        glBindTexture(GL_TEXTURE_CUBE_MAP, engine->gPBRManager->IrradianceSettings.prefilterMap);
        glActiveTexture(GL_TEXTURE21);
        glBindTexture(GL_TEXTURE_2D, engine->gPBRManager->IrradianceSettings.brdfLUTTexture);


        engine->GraphicsManager.UploadGlobalUniforms();
        Renderer.UpdateLightMatrix();

        if (!engine->GetEditorState())
        {
            if (engine->LightManager.EnableShadows)
            {
                for (auto const& entityID : mEntities)
                {
                    MeshComponent& Mesh = engine->world.GetComponent<MeshComponent>(entityID);
                    Renderer.RenderGameFromLightPOV(Mesh, entityID);
                }
            }

            for (auto const& entityID : mEntities)
            {
                auto& entCom = engine->world.GetComponent<EntityComponent>(entityID);
                if (!entCom.IsVisible) continue;

                //If No Mesh Component, Do not Continue
                if (!engine->world.CheckComponent<MeshComponent>(entityID)) { continue; }

                // If it is a base prefab, dont render
                if (engine->world.CheckComponent<PrefabComponent>(entityID))
                {
                    auto& prefab = engine->world.GetComponent<PrefabComponent>(entityID);

                    if (!prefab.IsInstance)
                        continue;
                }

                MeshComponent& Mesh = engine->world.GetComponent<MeshComponent>(entityID);

                if (Mesh.transparency == 0.0f) { continue; }

                Renderer.RenderGame(Mesh, entityID);
            }
        }
        else
        {
            if (engine->GraphicsManager.CheckRender == true)
            {
                // Estiamtion which models are in our frustrum
                //const auto& RenderablesVsFrustrum = engine->gCullingManager->ReturnContacted();

                /*************************************************************************
                  Render Without Stencer
                  Render Sky to Sceneview
                *************************************************************************/
                engine->MaterialManager.DoNotUpdateStencil();
                engine->GraphicsManager.RenderSky(FrameBufferMode::FBM_SCENE);

                if (engine->LightManager.EnableShadows == true)
                {
                    for (auto const& entityID : mEntities)
                    {
                        MeshComponent& Mesh = engine->world.GetComponent<MeshComponent>(entityID);
                        Renderer.RenderSceneFromLightPOV(Mesh, entityID);
                    }
                }

                for (auto const& entityID : mEntities)
                {
                    auto& entCom = engine->world.GetComponent<EntityComponent>(entityID);
                    if (!entCom.IsVisible) continue;

                    //If No Mesh Component, Do not Continue
                    if (!engine->world.CheckComponent<MeshComponent>(entityID)) { continue; }

                    // If it is a base prefab, dont render
                    if (engine->world.CheckComponent<PrefabComponent>(entityID))
                    {
                        auto& prefab = engine->world.GetComponent<PrefabComponent>(entityID);

                        if (!prefab.IsInstance)
                            continue;
                    }

                    // If CUlled off , dont render
                    if (engine->gCullingManager->ToRenderOrNot(entityID) == false) { continue; }

                    MeshComponent& Mesh = engine->world.GetComponent<MeshComponent>(entityID);

                    if (Mesh.transparency == 0.0f) { continue; }

                    // After hot-realoding , we check if he still exists or not
                    if (engine->AssimpManager.CheckGeometryExist(Mesh))
                    {
                        Renderer.RenderScene(Mesh, entityID);
                        Renderer.RenderGame(Mesh, entityID);
                        Renderer.RenderOtherViews(Mesh, entityID);
                        Renderer.Outline(Mesh, entityID, FrameBufferMode::FBM_SCENE);
                    }
                }

                engine->AssimpManager.MeshEditor_.Render();

                // Frustrum
                if (engine->GetEditorState() && engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
                {
                    engine->MaterialManager.DoNotUpdateStencil();
                    engine->gDebugManager.DrawDebugShapes(FrameBufferMode::FBM_SCENE);
                }

                engine->MaterialManager.StencilBufferClear();
            }
        }

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

        engine->gDebugDrawManager->Render();

        FrameMark
    }
}
