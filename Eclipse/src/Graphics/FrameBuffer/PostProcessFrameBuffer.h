#pragma once

namespace Eclipse
{
    enum class PostProcessType : unsigned int
    {
        PPT_NONE = 0,
        PPT_INVERSE = 1,
        PPT_GREY = 2,
        PPT_KERNEL = 3,
        PPT_BLUR = 4,
        PPT_SOBEL = 5,
        PPT_BLLEDING = 6,
        PPT_SCREENSHAKE = 7,
        PPT_MAXCOUNT
    };

    class PostProcess : public FrameBuffer
    {
    public:
        unsigned int rectVAO = 0;
        unsigned int rectVBO = 0;

        // Sobel
        float FadeInTimer = 0.0f;
        float Multiplier = 30.0f;
        float Visible = 1.0f;

        // Bleeding
        float BleedingTimer = 0.0f;

    public:
        PostProcessType PPType_ = PostProcessType::PPT_NONE;
        bool AllowPostProcess = false;

        void CreateFBO();
        void RenderToScreen();
        void PostProcessUpdate(FrameBufferMode);
        void SobelEffectUpdate(FrameBufferMode TargetFBO, FrameBufferMode RenderFBO);
        void FadeIn(PostProcessType Type, float& timer, float multiplier, FrameBufferMode WhichFBO);
    };
}