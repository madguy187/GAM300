#pragma once

namespace Eclipse
{
    class BloomEffect : public FrameBuffer
    {
    private:
        unsigned int BlurFBOs[2];
        unsigned int BlurColorbuffers[2];
        bool Horizontal = true;
        bool First_iteration = true;
        unsigned int Amount = 20;

    public:
        bool AllowBloom = false;

        void SetupGaussianBlurFBOs();
        void GaussianBlur(FrameBufferMode Mode);
    };
}