#include "pch.h"
#include "RenderManger.h"

namespace Eclipse
{
    void RenderManager::RenderScene(MeshComponent& Mesh, Entity entityID)
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
                // If no post process , just render normally
                engine->MaterialManager.UpdateStencilWithActualObject(entityID);

                if (engine->LightManager.EnableShadows == true)
                {
                    auto& ShadowMappingShader = Graphics::shaderpgms["PBRShader"];
                    ShadowMappingShader.Use();
                    glActiveTexture(GL_TEXTURE0 + 2);
                    ShadowMappingShader.setInt("shadowMap", 2);
                    glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(FrameBufferMode::FBM_SHADOW));
                }

                engine->AssimpManager.MeshDraw(Mesh, entityID,
                    FrameBufferMode::FBM_SCENE,
                    engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SCENE),
                    CameraComponent::CameraType::Editor_Camera);
            }

            // See Normal Vectors
            engine->MaterialManager.UpdateStencilWithActualObject(entityID);
            engine->AssimpManager.DebugNormals(Mesh, entityID, FrameBufferMode::FBM_SCENE, CameraComponent::CameraType::Editor_Camera);
        }
    }

    void RenderManager::RenderGame(MeshComponent& Mesh, Entity entityID)
    {
        // If Game View is visible
        if (engine->editorManager->GetEditorWindow<eGameViewWindow>()->IsVisible)
        {
            // If activated , we render to FBM_GAME_SOBEL then in the FrameBufferManager , we render the texture back to FBM_GAME
            if (engine->gFrameBufferManager->IsSobelEffect())
            {
                engine->MaterialManager.DoNotUpdateStencil();

                engine->AssimpManager.MeshDraw(Mesh, entityID,
                    FrameBufferMode::FBM_GAME_SOBEL,
                    engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME),
                    CameraComponent::CameraType::Game_Camera);
            }
            else
            {
                // If no post process , just render normally
                engine->MaterialManager.DoNotUpdateStencil();

                engine->AssimpManager.MeshDraw(Mesh, entityID,
                    FrameBufferMode::FBM_GAME,
                    engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME),
                    CameraComponent::CameraType::Game_Camera);
            }
        }
    }

    void RenderManager::RenderOtherViews(MeshComponent& Mesh, Entity entityID)
    {
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
            if (!engine->world.CheckComponent<AnimationComponent>(entityID))
            {
                //engine->MaterialManager.Highlight3DModels(entityID, FrameBufferMode::FBM_SCENE);
            }
        }
    }

    void RenderManager::UpdateLightMatrix()
    {
        auto& DLight_T = engine->world.GetComponent<TransformComponent>(engine->LightManager.DirectionLightID);

        lightProjection = glm::ortho(-100.0f, 100.0f, 100.0f, -100.0f, near_plane, far_plane);
        lightView = glm::lookAt(DLight_T.position.ConvertToGlmVec3Type(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        auto& simpleDepthShader = Graphics::shaderpgms["SimpleDepthShader"];
        simpleDepthShader.Use();

        GLint lightSpaceMatrix_ = simpleDepthShader.GetLocation("lightSpaceMatrix");
        glUniformMatrix4fv(lightSpaceMatrix_, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    }

    void RenderManager::RenderSceneFromLightPOV(MeshComponent& Mesh, Entity entityID)
    {
        engine->MaterialManager.DoNotUpdateStencil();
        engine->AssimpManager.RenderToDepth(Mesh, entityID, FrameBufferMode::FBM_SHADOW, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SHADOW), CameraComponent::CameraType::Editor_Camera);
    }

    void RenderManager::RenderGameFromLightPOV(MeshComponent& Mesh, Entity entityID)
    {
        engine->MaterialManager.DoNotUpdateStencil();
        engine->AssimpManager.RenderToDepth(Mesh, entityID, FrameBufferMode::FBM_SHADOW, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_SHADOW), CameraComponent::CameraType::Game_Camera);
    }

    void RenderManager::RenderSceneNormally(MeshComponent& Mesh, Entity entityID)
    {
        auto& Camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        auto& CameraPos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Editor_Camera));
        auto& ShadowMappingShader = Graphics::shaderpgms["ShadowMapping"];
        ShadowMappingShader.Use();

        GLint projection_ = ShadowMappingShader.GetLocation("projection");
        GLint view_ = ShadowMappingShader.GetLocation("view");

        glUniformMatrix4fv(projection_, 1, GL_FALSE, glm::value_ptr(Camera.projMtx));
        glUniformMatrix4fv(view_, 1, GL_FALSE, glm::value_ptr(Camera.viewMtx));

        GLint viewPos_ = ShadowMappingShader.GetLocation("viewPos");
        GLint lightPos_ = ShadowMappingShader.GetLocation("lightPos");
        GLint lightSpaceMatrix_1 = ShadowMappingShader.GetLocation("lightSpaceMatrix");

        //glUniform3f(viewPos_, CameraPos.position.getX(), CameraPos.position.getY(), CameraPos.position.getZ());
        //glUniform3f(lightPos_, lightPos.x, lightPos.y, lightPos.z);
        //glUniformMatrix4fv(lightSpaceMatrix_1, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        glActiveTexture(GL_TEXTURE0 + 2);
        ShadowMappingShader.setInt("shadowMap", 2);
        glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(FrameBufferMode::FBM_SHADOW));
        engine->AssimpManager.RenderFromDepth(Mesh, entityID, FrameBufferMode::FBM_SCENE, engine->gFrameBufferManager->GetRenderMode(FrameBufferMode::FBM_GAME),
            CameraComponent::CameraType::Editor_Camera);
    }
}
