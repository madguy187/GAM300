#include "pch.h"
#include "FrameBufferManager.h"

namespace Eclipse
{
    void FrameBufferManager::CreateFrameBuffers()
    {
        CreateFBO(1270, 593, FrameBufferMode::FBM_GAME);
        CreateFBO(1270, 593, FrameBufferMode::FBM_SCENE);
        CreateFBO(1270, 593, FrameBufferMode::FBM_TOP);
        CreateFBO(1270, 593, FrameBufferMode::FBM_BOTTOM);
        CreateFBO(1270, 593, FrameBufferMode::FBM_LEFT);
        CreateFBO(1270, 593, FrameBufferMode::FBM_RIGHT);
    }

    void FrameBufferManager::CreateFBO(unsigned int width_, unsigned int height_, FrameBufferMode in)
    {
        if (in == FrameBufferMode::FBM_NONE || in == FrameBufferMode::MAXCOUNT)
        {
            ENGINE_LOG_ASSERT(false, "Creating FrameBuffer with Invalid Type");
            std::exit(EXIT_FAILURE);
        }

        FrameBufferContainer.insert({in, std::make_shared<FrameBuffer>(width_, height_, in)});
        //ENGINE_CORE_INFO("FrameBuffer Ready For Use");
    }

    void FrameBufferManager::FrameBufferDraw()
    {
        FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_GAME)), "GameView");
        FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_SCENE)), "SceneView");
        FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_TOP)), "SceneView_Top");
        FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_BOTTOM)), "SceneView_Bottom");
        FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_LEFT)), "SceneView_Left");
        FrameBuffer::ShowWindow(*(GetFramebuffer(FrameBufferMode::FBM_RIGHT)), "SceneView_Right");
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

    void FrameBufferManager::UpdateAspectRatio(FrameBufferMode Mode , ECVec2 CurrentViewPortSize)
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
}