#include "pch.h"
#include "FrameBufferManager.h"

namespace Eclipse
{
    void FrameBufferManager::CreateFrameBuffers()
    {
        CreateFBO(OpenGL_Context::width, OpenGL_Context::height, FrameBufferMode::FBM_GAME);
        CreateFBO(OpenGL_Context::width, OpenGL_Context::height, FrameBufferMode::FBM_SCENE);
        CreateFBO(OpenGL_Context::width, OpenGL_Context::height, FrameBufferMode::FBM_TOP);
        CreateFBO(OpenGL_Context::width, OpenGL_Context::height, FrameBufferMode::FBM_BOTTOM);
        CreateFBO(OpenGL_Context::width, OpenGL_Context::height, FrameBufferMode::FBM_LEFT);
        CreateFBO(OpenGL_Context::width, OpenGL_Context::height, FrameBufferMode::FBM_RIGHT);
    }

    void FrameBufferManager::CreateFBO(unsigned int width_, unsigned int height_, FrameBufferMode in)
    {
        if (in == FrameBufferMode::FBM_NONE || in == FrameBufferMode::MAXCOUNT)
        {
            ENGINE_LOG_ASSERT(false, "Creating FrameBuffer with Invalid Type");
            std::exit(EXIT_FAILURE);
        }

        //std::unique_ptr<FrameBuffer> m_frameBuffer = std::make_unique<FrameBuffer>(width_, height_, in);
        FrameBufferContainer.insert({in, std::make_shared<FrameBuffer>(width_, height_, in)});
        ENGINE_CORE_INFO("FrameBuffer Ready For Use");
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
}