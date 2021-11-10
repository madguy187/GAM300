#include "pch.h"
#include "RenderSystem.h"

// Views
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

        // Load All Compilers ====================== =======
        engine->gEngineCompiler->Init();

        // Create SKY =============================
        engine->GraphicsManager.CreateSky("src/Assets/Sky");

        // DebugManagerRender
        engine->gDebugDrawManager->Init();

        auto& ShadowMappingShader = Graphics::shaderpgms["ShadowMapping"];
        ShadowMappingShader.Use();
        ShadowMappingShader.Use();
        ShadowMappingShader.setInt("diffuseTexture", 0);
        ShadowMappingShader.setInt("shadowMap", 2);
    }

    void RenderSystem::Update()
    {
        ZoneScopedN("Render System")
            engine->Timer.SetName({ SystemName::RENDER });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        engine->GraphicsManager.UploadGlobalUniforms();
        Renderer.UpdateLightMatrix();

        if (engine->GraphicsManager.CheckRender == true)
        {
            // Estiamtion which models are in our frustrum
            //const auto& RenderablesVsFrustrum = engine->gCullingManager->ReturnContacted();

            engine->MaterialManager.DoNotUpdateStencil();
            engine->GraphicsManager.RenderSky(FrameBufferMode::FBM_SCENE);


            for (auto const& entityID : mEntities)
            {
                MeshComponent& Mesh = engine->world.GetComponent<MeshComponent>(entityID);
                Renderer.RenderSceneFromLightPOV(Mesh, entityID);
            }

            for (auto const& entityID : mEntities)
            {
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

                    //Renderer.RenderSceneNormally(Mesh, entityID);
                    Renderer.RenderGame(Mesh, entityID);
                    Renderer.RenderOtherViews(Mesh, entityID);
                }
            }

            engine->AssimpManager.MeshEditor_.Render();

            // Frustrum
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