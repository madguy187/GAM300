#include "pch.h"
#include "PostProcessFrameBuffer.h"

namespace Eclipse
{
    void PostProcess::CreateFBO()
    {
        float RectangleVertices[] =
        {
            // Coords    // texCoords
             1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,

             1.0f,  1.0f,  1.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f,  0.0f, 1.0f
        };

        auto& shdrpgm = Graphics::shaderpgms["framebuffer"];
        shdrpgm.Use();
        glUniform1i(glGetUniformLocation(shdrpgm.GetHandle(), "screenTexture"), 0);

        // Prepare framebuffer rectangle VBO and VAO
        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(RectangleVertices), &RectangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    void PostProcess::RenderToScreen()
    {
        glBindVertexArray(rectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void PostProcess::PostProcessUpdate(FrameBufferMode Scene)
    {
        if (engine->editorManager->GetEditorWindow<SceneWindow>()->IsVisible)
        {
            if (AllowPostProcess == false)  return;

            if (PPType_ == PostProcessType::PPT_NONE) return;

            if (PPType_ == PostProcessType::PPT_SOBEL) return;

            if (PPType_ == PostProcessType::PPT_BLLEDING)
            {
                BleedingTimer += engine->Game_Clock.get_fixedDeltaTime() * 5;
            }

            engine->gFrameBufferManager->UseFrameBuffer(Scene);

            auto& shdrpgm = Graphics::shaderpgms["PostProcess"];
            shdrpgm.Use();

            GLint Inversion = shdrpgm.GetLocation("Type");
            GLint iTime_ = shdrpgm.GetLocation("iTime");

            GLCall(glUniform1i(Inversion, static_cast<GLint>(PPType_)));
            GLCall(glUniform1f(iTime_, BleedingTimer));

            glBindVertexArray(rectVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(Scene));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void PostProcess::SobelEffectUpdate(FrameBufferMode TargetFBO, FrameBufferMode RenderFBO)
    {
        if (AllowPostProcess == true)
        {
            if (PPType_ == PostProcessType::PPT_SOBEL)
            {
                FadeIn(PostProcessType::PPT_SOBEL, FadeInTimer, Multiplier, RenderFBO);

                // We will output to Game FrameBuffer
                //UseFrameBuffer(RenderFBO);

                if (engine->GetEditorState() == true)
                {
                    // We will output to Game FrameBuffer
                    engine->gFrameBufferManager->UseFrameBuffer(RenderFBO);
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

                GLCall(glUniform1i(Inversion, static_cast<GLint>(PPType_)));
                GLCall(glUniform1i(Height_, engine->gFrameBufferManager->FrameBufferContainer[RenderFBO]->m_height));
                GLCall(glUniform1i(Width_, engine->gFrameBufferManager->FrameBufferContainer[RenderFBO]->m_width));

                if (RenderFBO == FrameBufferMode::FBM_SCENE)
                {
                    GLCall(glUniform1f(FadeInTimer_, Visible));
                }
                else
                {
                    GLCall(glUniform1f(FadeInTimer_, FadeInTimer));
                }

                glBindVertexArray(rectVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, engine->gFrameBufferManager->GetTextureID(TargetFBO));
                glDrawArrays(GL_TRIANGLES, 0, 6);

                shdrpgm.UnUse();
            }
        }
    }

    void PostProcess::FadeIn(PostProcessType Type, float& timer, float multiplier, FrameBufferMode WhichFBO)
    {
        (void)WhichFBO;
        (void)multiplier;
        (void)Type;

        timer = 1.0f;

        //if (PostProcess->PPType_ == Type && timer <= 1.0f)
        //{
        //    timer += (engine->Game_Clock.get_fixedDeltaTime() / multiplier);
        //}
    }
}