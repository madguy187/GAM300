#include "pch.h"
#include "Graphics/FrameBuffer/EmissiveFrameBuffer.h"

namespace Eclipse
{
    void BloomEffect::SetupGaussianBlurFBOs()
    {
        glGenFramebuffers(2, BlurFBOs);
        glGenTextures(2, BlurColorbuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, BlurFBOs[i]);
            glBindTexture(GL_TEXTURE_2D, BlurColorbuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1270, 593, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BlurColorbuffers[i], 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cout << "Framebuffer not complete!" << std::endl;
            }
        }
    }

    void BloomEffect::GaussianBlur(FrameBufferMode Mode)
    {
        if (AllowBloom == true)
        {
            auto& shdrpgm3 = Graphics::shaderpgms["Blur"];
            shdrpgm3.Use();
            Horizontal = true, First_iteration = true;
            shdrpgm3.setInt("image", 0);

            engine->MaterialManager.DoNotUpdateStencil();
            for (unsigned int i = 0; i < Amount; i++)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, BlurFBOs[Horizontal]);
                shdrpgm3.setInt("horizontal", Horizontal);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, First_iteration ?
                    engine->gFrameBufferManager->GetFramebuffer(Mode)->m_data.ColorBuffers[1] :
                    BlurColorbuffers[!Horizontal]);

                engine->gFrameBufferManager->RenderToScreen();

                Horizontal = !Horizontal;

                if (First_iteration) First_iteration = false;
            }

            engine->gFrameBufferManager->UseFrameBuffer(Mode);
            engine->MaterialManager.DoNotUpdateStencil();

            auto& shdrpgm4 = Graphics::shaderpgms["BloomFinal"];
            shdrpgm4.Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(Mode));
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, BlurColorbuffers[!Horizontal]);
            shdrpgm4.setInt("bloom", true);
            shdrpgm4.setFloat("exposure", 1.0f);
            shdrpgm4.setInt("scene", 0);
            shdrpgm4.setInt("bloomBlur", 1);

            engine->gFrameBufferManager->RenderToScreen();
            engine->MaterialManager.StencilBufferClear();
        }
    }
}