#include "pch.h"
#include "FrameBufferManager.h"

namespace Eclipse
{
    void FrameBufferManager::CreateFrameBuffers()
    {
        if (engine->GetEditorState() == true)
        {
            // Basic FrameBuffers
            CreateFBO(1270, 593, FrameBufferMode::FBM_GAME);
            CreateFBO(1270, 593, FrameBufferMode::FBM_SCENE);
            CreateFBO(1270, 593, FrameBufferMode::FBM_TOP);
            CreateFBO(1270, 593, FrameBufferMode::FBM_BOTTOM);
            CreateFBO(1270, 593, FrameBufferMode::FBM_LEFT);
            CreateFBO(1270, 593, FrameBufferMode::FBM_RIGHT);

            // Editor Stuffs
            CreateFBO(1270, 593, FrameBufferMode::FBM_MATERIALEDITOR);
            CreateFBO(1270, 593, FrameBufferMode::FBM_MESHEDITOR);

            // Sobel Framebuffers
            // We Render to this FBO then we will render the effect into scene view.
            CreateFBO(1270, 593, FrameBufferMode::FBM_GAME_SOBEL);
            CreateFBO(1270, 593, FrameBufferMode::FBM_SCENE_SOBEL);

            // Additionals - PostProcess
            PostProcess = std::make_unique<FrameBuffer>();
            PostProcess->CreatePostProcessFramebuffer();

            // Shadow FBO
            CreateFBO(1270, 593, FrameBufferMode::FBM_SHADOW);
        }
        else
        {
            // Basic FrameBuffers
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_GAME);
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_SCENE);
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_TOP);
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_BOTTOM);
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_LEFT);
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_RIGHT);

            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_MATERIALEDITOR);
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_MESHEDITOR);

            // Sobel Framebuffers
            // We Render to this FBO then we will render the effect into scene view.
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_GAME_SOBEL);
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_SCENE_SOBEL);

            // Additionals - PostProcess
            PostProcess = std::make_unique<FrameBuffer>();
            PostProcess->CreatePostProcessFramebuffer();

            // Shadow FBO
            CreateFBO(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), FrameBufferMode::FBM_SHADOW);
        }
    }

    void FrameBufferManager::CreateFBO(unsigned int width_, unsigned int height_, FrameBufferMode in)
    {
        if (in == FrameBufferMode::FBM_NONE || in == FrameBufferMode::MAXCOUNT)
        {
            ENGINE_LOG_ASSERT(false, "Creating FrameBuffer with Invalid Type");
            std::exit(EXIT_FAILURE);
        }

        FrameBufferContainer.insert({ in, std::make_shared<FrameBuffer>(width_, height_, in) });
        UpdateAspectRatio(in, glm::vec2(width_, height_));
    }

    void FrameBufferManager::FrameBufferDraw()
    {
        if (engine->GetEditorState() == false)
        {
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_GAME)));
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_SCENE)));
        }
        else
        {
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_GAME)));
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_SCENE)));
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_TOP)));
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_BOTTOM)));
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_LEFT)));
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_RIGHT)));

            //Editor Stuffs
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_MATERIALEDITOR)));
            FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_MESHEDITOR)));
        }
    }

    FrameBuffer* FrameBufferManager::GetFramebuffer(FrameBufferMode mode)
    {
        if (mode == FrameBufferMode::MAXCOUNT || mode == FrameBufferMode::FBM_NONE)
        {
            ENGINE_LOG_ASSERT(false, " Wrong FrameBuffer Type");
            std::exit(EXIT_FAILURE);
        }
        return FrameBufferContainer[mode].get();
    }

    void FrameBufferManager::UseFrameBuffer(FrameBufferMode mode)
    {
        FrameBufferContainer[mode]->Bind();
    }

    void FrameBufferManager::UnBind(FrameBufferMode mode)
    {
        FrameBufferContainer[mode]->Unbind();
    }

    unsigned int FrameBufferManager::GetFrameBufferID(FrameBufferMode mode)
    {
        return FrameBufferContainer[mode]->GetFrameBufferID();
    }

    unsigned int FrameBufferManager::GetTextureID(FrameBufferMode mode)
    {
        return FrameBufferContainer[mode]->GetTextureColourBufferID();
    }

    unsigned int FrameBufferManager::GetSecondTextureID(FrameBufferMode mode)
    {
        return FrameBufferContainer[mode]->m_data.ColorBuffers[1];
    }

    RenderMode FrameBufferManager::GetRenderMode(FrameBufferMode mode)
    {
        return FrameBufferContainer[mode]->GetRenderMode();
    }

    bool FrameBufferManager::CheckFrameBuffer(unsigned int ID, FrameBufferMode mode)
    {
        if (ID == FrameBufferContainer[mode]->GetFrameBufferID())
            return true;

        return false;
    }

    void FrameBufferManager::GlobalBind()
    {
        if (engine->GetEditorState() == true)
        {
            for (auto& i : FrameBufferContainer)
            {
                i.second->Bind();
                i.second->Clear();
            }
        }
        else
        {
            FrameBufferContainer[FrameBufferMode::FBM_GAME]->Bind();
            FrameBufferContainer[FrameBufferMode::FBM_GAME]->Clear();
            FrameBufferContainer[FrameBufferMode::FBM_GAME_SOBEL]->Bind();
            FrameBufferContainer[FrameBufferMode::FBM_GAME_SOBEL]->Clear();
        }
    }

    void FrameBufferManager::DeleteFrameBuffer(FrameBufferMode Mode)
    {
        FrameBufferContainer.erase(Mode);
    }

    float FrameBufferManager::GetAspectRatio(FrameBufferMode Mode)
    {
        return FrameBufferContainer[Mode]->AspectRatio;
    }

    void FrameBufferManager::UpdateAspectRatio(FrameBufferMode Mode, ECVec2 CurrentViewPortSize)
    {
        FrameBufferContainer[Mode]->AspectRatio = CurrentViewPortSize.getX() / CurrentViewPortSize.getY();
    }

    void FrameBufferManager::UpdateAspectRatio(FrameBufferMode Mode, glm::vec2 CurrentViewPortSize)
    {
        FrameBufferContainer[Mode]->AspectRatio = CurrentViewPortSize.x / CurrentViewPortSize.y;
    }

    void FrameBufferManager::MainWindowSettings()
    {
        glViewport(0, 0, OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight());
        engine->GraphicsManager.SetBackGroundColour();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    unsigned int FrameBufferManager::GetFrameBufferWidth(FrameBufferMode in)
    {
        return FrameBufferContainer[in]->m_width;
    }

    unsigned int FrameBufferManager::GetFrameBufferHeight(FrameBufferMode in)
    {
        return FrameBufferContainer[in]->m_height;
    }

    float FrameBufferManager::GetAspectRatio(CameraComponent::CameraType in)
    {
        switch (in)
        {
        case CameraComponent::CameraType::MeshEditor_Camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_MESHEDITOR]->AspectRatio;
        }
        break;

        case CameraComponent::CameraType::MaterialEditor_Camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_MATERIALEDITOR]->AspectRatio;
        }
        break;

        case CameraComponent::CameraType::Editor_Camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_SCENE]->AspectRatio;
        }
        break;

        case CameraComponent::CameraType::Game_Camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_GAME]->AspectRatio;
        }
        break;

        case CameraComponent::CameraType::TopView_Camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_TOP]->AspectRatio;
        }
        break;

        case CameraComponent::CameraType::BottomView_Camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_BOTTOM]->AspectRatio;
        }
        break;

        case CameraComponent::CameraType::RightView_camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_RIGHT]->AspectRatio;
        }
        break;

        case CameraComponent::CameraType::LeftView_Camera:
        {
            return FrameBufferContainer[FrameBufferMode::FBM_LEFT]->AspectRatio;
        }
        break;
        }

        return 0.0f;
    }

    FrameBufferMode FrameBufferManager::GetFrameBufferMode(CameraComponent::CameraType in)
    {
        switch (in)
        {
        case CameraComponent::CameraType::MeshEditor_Camera:
        {
            return FrameBufferMode::FBM_MESHEDITOR;
        }
        break;

        case CameraComponent::CameraType::MaterialEditor_Camera:
        {
            return FrameBufferMode::FBM_MATERIALEDITOR;
        }
        break;

        case CameraComponent::CameraType::Editor_Camera:
        {
            return FrameBufferMode::FBM_SCENE;
        }
        break;

        case CameraComponent::CameraType::Game_Camera:
        {
            return FrameBufferMode::FBM_GAME;
        }
        break;

        case CameraComponent::CameraType::TopView_Camera:
        {
            return FrameBufferMode::FBM_TOP;
        }
        break;

        case CameraComponent::CameraType::BottomView_Camera:
        {
            return FrameBufferMode::FBM_BOTTOM;
        }
        break;

        case CameraComponent::CameraType::RightView_camera:
        {
            return FrameBufferMode::FBM_RIGHT;
        }
        break;

        case CameraComponent::CameraType::LeftView_Camera:
        {
            return FrameBufferMode::FBM_LEFT;
        }
        break;
        }

        return FrameBufferMode::MAXCOUNT;
    }

    void FrameBufferManager::FadeIn(FrameBuffer::PostProcessType Type, float& timer, float multiplier, FrameBufferMode WhichFBO)
    {
        (void)WhichFBO;

        timer = 1.0f;

        //if (PostProcess->PPType_ == Type && timer <= 1.0f)
        //{
        //    timer += (engine->Game_Clock.get_fixedDeltaTime() / multiplier);
        //}
    }

    void FrameBufferManager::PostProcessUpdate(FrameBufferMode Scene)
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (PostProcess->AllowPostProcess == false)
                return;

            if (PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_NONE)
                return;

            if (PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_SOBEL)
                return;

            if (PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_BLLEDING)
            {
                PostProcess->BleedingTimer += engine->Game_Clock.get_fixedDeltaTime() * 5;
            }

            engine->gFrameBufferManager->UseFrameBuffer(Scene);

            auto& shdrpgm = Graphics::shaderpgms["PostProcess"];
            shdrpgm.Use();

            GLint Inversion = shdrpgm.GetLocation("Type");
            GLint iTime_ = shdrpgm.GetLocation("iTime");

            GLCall(glUniform1i(Inversion, static_cast<GLint>(PostProcess->PPType_)));
            GLCall(glUniform1f(iTime_, PostProcess->BleedingTimer));

            glBindVertexArray(PostProcess->rectVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(Scene));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void FrameBufferManager::Reset()
    {
        PostProcess->AllowPostProcess = false;
        PostProcess->FadeInTimer = 0.0f;
        PostProcess->PPType_ = FrameBuffer::PostProcessType::PPT_NONE;
    }

    void FrameBufferManager::SetSobelEffect()
    {
        PostProcess->AllowPostProcess = true;
        PostProcess->FadeInTimer = 0.0f;
        PostProcess->PPType_ = FrameBuffer::PostProcessType::PPT_SOBEL;
    }

    void FrameBufferManager::SobelEffectUpdate(FrameBufferMode TargetFBO, FrameBufferMode RenderFBO)
    {
        if (PostProcess->AllowPostProcess == true)
        {
            if (PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_SOBEL)
            {
                FadeIn(FrameBuffer::PostProcessType::PPT_SOBEL, PostProcess->FadeInTimer, PostProcess->Multiplier, RenderFBO);

                // We will output to Game FrameBuffer
                //UseFrameBuffer(RenderFBO);

                if (engine->GetEditorState() == true)
                {
                    // We will output to Game FrameBuffer
                    UseFrameBuffer(RenderFBO);
                }
                else
                {
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glViewport(0, 0, OpenGL_Context::width, OpenGL_Context::height);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                }

                auto& shdrpgm = Graphics::shaderpgms["PostProcess"];
                shdrpgm.Use();

                GLint Inversion = shdrpgm.GetLocation("Type");
                GLint Height_ = shdrpgm.GetLocation("Height");
                GLint Width_ = shdrpgm.GetLocation("Width");
                GLint FadeInTimer_ = shdrpgm.GetLocation("FadeInTimer");

                GLCall(glUniform1i(Inversion, static_cast<GLint>(PostProcess->PPType_)));
                GLCall(glUniform1i(Height_, engine->gFrameBufferManager->FrameBufferContainer[RenderFBO]->m_height));
                GLCall(glUniform1i(Width_, engine->gFrameBufferManager->FrameBufferContainer[RenderFBO]->m_width));

                if (RenderFBO == FrameBufferMode::FBM_SCENE)
                {
                    GLCall(glUniform1f(FadeInTimer_, PostProcess->Visible));
                }
                else
                {
                    GLCall(glUniform1f(FadeInTimer_, PostProcess->FadeInTimer));
                }

                glBindVertexArray(PostProcess->rectVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(TargetFBO));
                glDrawArrays(GL_TRIANGLES, 0, 6);

                shdrpgm.UnUse();
            }
        }
    }

    bool FrameBufferManager::IsSobelEffect()
    {
        return (PostProcess->AllowPostProcess && PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_SOBEL);
    }

    void FrameBufferManager::PostProcessUpdate()
    {
        if (engine->GetEditorState() == false)
        {
            PostProcess->AllowPostProcess = true;
            PostProcess->PPType_ = FrameBuffer::PostProcessType::PPT_SOBEL;
            SobelEffectUpdate(FrameBufferMode::FBM_GAME_SOBEL, FrameBufferMode::FBM_GAME);
        }
        else
        {
            if (PostProcess->AllowPostProcess == true)
            {
                if (PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_SOBEL)
                {
                    if (engine->IsScenePlaying() == true)
                    {
                        SobelEffectUpdate(FrameBufferMode::FBM_GAME_SOBEL, FrameBufferMode::FBM_GAME);
                    }

                    SobelEffectUpdate(FrameBufferMode::FBM_GAME_SOBEL, FrameBufferMode::FBM_GAME);
                    SobelEffectUpdate(FrameBufferMode::FBM_SCENE_SOBEL, FrameBufferMode::FBM_SCENE);
                }
                else
                {
                    PostProcessUpdate(FrameBufferMode::FBM_SCENE);
                }
            }
        }

        //For Darren to Check Depth Map
        //engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_GAME);
        //auto& debugDepthQuad = Graphics::shaderpgms["DepthQuad"];
        //debugDepthQuad.Use();
        //debugDepthQuad.setFloat("near_plane", 1);
        //debugDepthQuad.setFloat("far_plane", 100.0f);

        //glBindVertexArray(engine->gFrameBufferManager->PostProcess->rectVAO);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(FrameBufferMode::FBM_SHADOW));
        //glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}