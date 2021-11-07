#pragma once

namespace Eclipse
{
    class FrameBufferManager
    {
    public:
        std::unordered_map <FrameBufferMode, std::shared_ptr<FrameBuffer>> FrameBufferContainer;

        // Seperate FBO for Post Process
        std::unique_ptr<FrameBuffer> PostProcess;

        FrameBufferManager() {};
        void FrameBufferDraw();
        FrameBuffer* GetFramebuffer(FrameBufferMode mode);
        void UseFrameBuffer(FrameBufferMode mode);
        void UnBind(FrameBufferMode mode);
        unsigned int GetFrameBufferID(FrameBufferMode mode);
        unsigned int GetTextureID(FrameBufferMode mode);
        unsigned int GetSecondTextureID(FrameBufferMode mode);
        RenderMode GetRenderMode(FrameBufferMode mode);
        bool CheckFrameBuffer(unsigned int ID, FrameBufferMode mode);
        void GlobalBind();
        void DeleteFrameBuffer(FrameBufferMode Mode);
        float GetAspectRatio(FrameBufferMode Mode);
        void UpdateAspectRatio(FrameBufferMode Mode, ECVec2 CurrentViewPortSize);
        void UpdateAspectRatio(FrameBufferMode Mode, glm::vec2 CurrentViewPortSize);
        void MainWindowSettings();
        unsigned int GetFrameBufferWidth(FrameBufferMode in);
        unsigned int GetFrameBufferHeight(FrameBufferMode in);
        float GetAspectRatio(CameraComponent::CameraType);
        FrameBufferMode GetFrameBufferMode(CameraComponent::CameraType in);
        void CreateFrameBuffers();
        void CreateFBO(unsigned int Height, unsigned int Width, FrameBufferMode in);

        void PostProcessUpdate();
        bool IsSobelEffect();
        void Reset();
        void SetSobelEffect();

    private:
        // Effects
        void FadeIn(FrameBuffer::PostProcessType Type, float& timer, float multiplier, FrameBufferMode WhichFBO);
        void PostProcessUpdate(FrameBufferMode);
        void SobelEffectUpdate(FrameBufferMode TargetFBO, FrameBufferMode RenderFBO);
    };
}