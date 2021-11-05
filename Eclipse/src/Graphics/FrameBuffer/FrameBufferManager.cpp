#include "pch.h"
#include "FrameBufferManager.h"

namespace Eclipse
{
    void FrameBufferManager::CreateFrameBuffers()
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

        // PostProcess
        PostProcess = std::make_unique<FrameBuffer>();
        PostProcess->CreatePostProcessFramebuffer();

        // Create additional Buffer For Game View
        CreateFBO(1270, 593, FrameBufferMode::FBM_GAME_SOBEL);
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
        for (auto& i : FrameBufferContainer)
        {
            i.second->Bind();
            i.second->Clear();
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

    void FrameBufferManager::FadeIn(FrameBuffer::PostProcessType Type, float& timer , float multiplier)
    {
        if (engine->IsScenePlaying() == true)
        {
            if (PostProcess->PPType_ == Type && timer <= 1.0f)
            {
                timer += ( engine->Game_Clock.get_fixedDeltaTime() / multiplier );
            }
        }
        else
        {
            timer = 0.0f;
        }
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

            engine->gFrameBufferManager->UseFrameBuffer(Scene);

            auto& shdrpgm = Graphics::shaderpgms["PostProcess"];
            shdrpgm.Use();

            GLint Inversion = shdrpgm.GetLocation("Type");
            GLint Height_ = shdrpgm.GetLocation("Height");
            GLint Width_ = shdrpgm.GetLocation("Width");
            GLint FadeInTimer_ = shdrpgm.GetLocation("FadeInTimer");

            GLCall(glUniform1i(Inversion, static_cast<GLint>(PostProcess->PPType_)));

            glBindVertexArray(PostProcess->rectVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(Scene));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void FrameBufferManager::SobelEffectUpdate()
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (PostProcess->AllowPostProcess == true)
            {
                if (PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_SOBEL)
                {
                    FadeIn(FrameBuffer::PostProcessType::PPT_SOBEL, PostProcess->FadeInTimer, PostProcess->Multiplier);

                    // We will output to Game FrameBuffer
                    engine->gFrameBufferManager->UseFrameBuffer(FrameBufferMode::FBM_GAME);

                    auto& shdrpgm = Graphics::shaderpgms["PostProcess"];
                    shdrpgm.Use();

                    GLint Inversion = shdrpgm.GetLocation("Type");
                    GLint Height_ = shdrpgm.GetLocation("Height");
                    GLint Width_ = shdrpgm.GetLocation("Width");
                    GLint FadeInTimer_ = shdrpgm.GetLocation("FadeInTimer");

                    GLCall(glUniform1i(Inversion, static_cast<GLint>(PostProcess->PPType_)));
                    GLCall(glUniform1i(Height_, engine->gFrameBufferManager->FrameBufferContainer[FrameBufferMode::FBM_GAME]->m_height));
                    GLCall(glUniform1i(Width_, engine->gFrameBufferManager->FrameBufferContainer[FrameBufferMode::FBM_GAME]->m_width));
                    GLCall(glUniform1f(FadeInTimer_, PostProcess->FadeInTimer));

                    glBindVertexArray(PostProcess->rectVAO);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(FrameBufferMode::FBM_GAME_SOBEL));
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
            }
        }
    }

    void FrameBufferManager::PostProcessUpdate()
    {
        if (PostProcess->AllowPostProcess == true)
        {
            if (PostProcess->PPType_ == FrameBuffer::PostProcessType::PPT_SOBEL)
            {
                SobelEffectUpdate();
            }
            else
            {
                PostProcessUpdate(FrameBufferMode::FBM_SCENE);
            }
        }
    }
}