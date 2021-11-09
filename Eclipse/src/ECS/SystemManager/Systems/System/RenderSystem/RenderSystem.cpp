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

glm::vec3 lightPos(-20.0f, 40.0f, -10.0f);
glm::mat4 lightProjection, lightView;
glm::mat4 lightSpaceMatrix;

namespace Eclipse
{
    unsigned int quadVAO = 0;
    unsigned int quadVBO;

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

        auto& ShadowMappingShader = Graphics::shaderpgms["ShadowMapping"];
        ShadowMappingShader.Use();
        ShadowMappingShader.Use();
        ShadowMappingShader.setInt("diffuseTexture", 0);
        ShadowMappingShader.setInt("shadowMap", 16);
    }

    void RenderSystem::Update()
    {
        float near_plane = 1.0f, far_plane = 500.5f;

        ZoneScopedN("Render System")
            engine->Timer.SetName({ SystemName::RENDER });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        engine->GraphicsManager.UploadGlobalUniforms();

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

            // Basic Primitives Render Start =============================
            for (auto const& entityID : mEntities)
            {
                // Used somewhere else.
                //if (entityID == engine->gPBRManager->gMaterialEditorSettings->InnerEntity || entityID == engine->gPBRManager->gMaterialEditorSettings->OuterEntity)
                //    continue;

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

                if (Mesh.transparency == 0.0f)
                {
                    continue;
                }

                // After hot-realoding , we check if he still exists or not
                if (engine->AssimpManager.CheckGeometryExist(Mesh))
                {
                    // If Scene View is visible
                    if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
                    {
                        // If PostProcess::Sobel is activated
                        if (engine->gFrameBufferManager->IsSobelEffect())
                        {
                            // If activated , we render to FBM_SCENE_SOBEL then in the FrameBufferManager , we render the texture back to FBM_SCENE
                            engine->MaterialManager.UpdateStencilWithActualObject(entityID);

                            engine->AssimpManager.MeshDraw(Mesh, entityID,
                                FrameBufferMode::FBM_SCENE_SOBEL,
                                engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SCENE_SOBEL),
                                CameraComponent::CameraType::Editor_Camera);
                        }
                        else
                        {
                            //If no post process , just render normally
                            //engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                            //
                            //engine->AssimpManager.MeshDraw(Mesh, entityID,
                            //FrameBufferMode::FBM_SCENE,
                            //engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SCENE),
                            //CameraComponent::CameraType::Editor_Camera);
                        }

                        // See Normal Vectors
                        engine->MaterialManager.UpdateStencilWithActualObject(entityID);
                        engine->AssimpManager.DebugNormals(Mesh, entityID, FrameBufferMode::FBM_SCENE, CameraComponent::CameraType::Editor_Camera);
                    }

                    // If Game View is visible
                    if (engine->editorManager->GetEditorWindow<eGameViewWindow>()->IsVisible)
                    {
                        // If activated , we render to FBM_GAME_SOBEL then in the FrameBufferManager , we render the texture back to FBM_GAME
                        if (engine->gFrameBufferManager->IsSobelEffect())
                        {
                            engine->MaterialManager.DoNotUpdateStencil();

                            engine->AssimpManager.MeshDraw(Mesh, entityID,
                                FrameBufferMode::FBM_GAME_SOBEL,
                                engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME_SOBEL),
                                CameraComponent::CameraType::Game_Camera);
                        }
                        else
                        {
                            // If no post process , just render normally
                            engine->MaterialManager.DoNotUpdateStencil();

                            //engine->AssimpManager.MeshDraw(Mesh, entityID,
                            //    FrameBufferMode::FBM_GAME,
                            //    engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME),
                            //    CameraComponent::CameraType::Game_Camera);
                        }
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

                    // If scene not playing , we enable highlight
                    if (engine->IsScenePlaying() != true)
                    {
                        //engine->MaterialManager.Highlight3DModels(entityID, FrameBufferMode::FBM_SCENE);
                    }

                    // 1. render depth of scene to texture (from light's perspective)
                    // -------------------------------------------------------------
                    //glm::vec3 lightPos(-20.0f, 40.0f, -10.0f);
                    //glm::mat4 lightProjection, lightView;
                    //glm::mat4 lightSpaceMatrix;
                    lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
                    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
                    lightSpaceMatrix = lightProjection * lightView;

                    engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SHADOW);

                    // render scene from light's point of view
                    auto& simpleDepthShader = Graphics::shaderpgms["SimpleDepthShader"];
                    simpleDepthShader.Use();

                    GLint lightSpaceMatrix_ = simpleDepthShader.GetLocation("lightSpaceMatrix");
                    glUniformMatrix4fv(lightSpaceMatrix_, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    engine->MaterialManager.DoNotUpdateStencil();
                    engine->AssimpManager.LightPerSpectiveDraw(Mesh, entityID, FrameBufferMode::FBM_SHADOW, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SHADOW),
                        CameraComponent::CameraType::Editor_Camera);

                    glActiveTexture(GL_TEXTURE16);
                    glBindTexture(GL_TEXTURE_2D, 0);

                    simpleDepthShader.UnUse();

                    // 2. render scene as normal using the generated depth/shadow map  
                    // --------------------------------------------------------------
                    engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_SCENE);
                    auto& Camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
                    auto& CameraPos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
                    auto& ShadowMappingShader = Graphics::shaderpgms["ShadowMapping"];
                    ShadowMappingShader.Use();

                    GLint projection_ = ShadowMappingShader.GetLocation("projection");
                    GLint view_ = ShadowMappingShader.GetLocation("view");

                    glUniformMatrix4fv(projection_, 1, GL_FALSE, glm::value_ptr(Camera.projMtx));
                    glUniformMatrix4fv(view_, 1, GL_FALSE, glm::value_ptr(Camera.viewMtx));

                    // set light uniforms

                    if (engine->gFrameBufferManager->PostProcess->AllowPostProcess)
                    {
                        GLint viewPos_ = ShadowMappingShader.GetLocation("viewPos");
                        GLint lightPos_ = ShadowMappingShader.GetLocation("lightPos");
                        GLint lightSpaceMatrix_1 = ShadowMappingShader.GetLocation("lightSpaceMatrix");

                        glUniform3f(viewPos_, CameraPos.position.getX(), CameraPos.position.getY(), CameraPos.position.getZ());
                        glUniform3f(lightPos_, lightPos.x, lightPos.y, lightPos.z);
                        glUniformMatrix4fv(lightSpaceMatrix_1, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

                        glActiveTexture(GL_TEXTURE0);
                        glActiveTexture(GL_TEXTURE16);
                        engine->MaterialManager.DoNotUpdateStencil();
                        glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(FrameBufferMode::FBM_SHADOW));
                        engine->AssimpManager.ShadowDraw(Mesh, entityID, FrameBufferMode::FBM_SCENE, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME),
                            CameraComponent::CameraType::Editor_Camera);

                        ShadowMappingShader.UnUse();
                    }
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