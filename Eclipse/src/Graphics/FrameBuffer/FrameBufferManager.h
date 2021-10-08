#pragma once

namespace Eclipse
{
    class FrameBufferManager
    {
    public:
        std::unordered_map <FrameBufferMode, std::shared_ptr<FrameBuffer>> FrameBufferContainer;

        FrameBufferManager() {};
        void CreateFrameBuffers();
        void CreateFBO(unsigned int Height, unsigned int Width, FrameBufferMode in);
        void FrameBufferDraw();
        FrameBuffer* GetFramebuffer(FrameBufferMode mode);
        void UseFrameBuffer(FrameBufferMode mode);
        void UnBind(FrameBufferMode mode);
        unsigned int GetFrameBufferID(FrameBufferMode mode);
        unsigned int GetTextureID(FrameBufferMode mode);
        RenderMode GetRenderMode(FrameBufferMode mode);
        bool CheckFrameBuffer(unsigned int ID, FrameBufferMode mode);
        void GlobalBind();
        void DeleteFrameBuffer(FrameBufferMode Mode);
        float GetAspectRatio(FrameBufferMode Mode);
        void UpdateAspectRatio(FrameBufferMode Mode, ECVec2 CurrentViewPortSize);
        void UpdateAspectRatio(FrameBufferMode Mode, glm::vec2 CurrentViewPortSize);
        void MainWindowSettings();
    };
}