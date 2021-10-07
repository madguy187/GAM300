#include "pch.h"
#include "FrameBuffer.h"

namespace Eclipse
{
    FrameBuffer::FrameBuffer(const glm::uvec2& p_size, FrameBufferMode in) :
        m_size{ p_size },
        m_width{ m_size.x },
        m_height{ m_size.y },
        FrameBufferType{ in }
    {
        m_data.hiddentype = in;
        CreateFrameBuffer(m_width, m_height);
        ENGINE_CORE_INFO("FrameBuffer Created Successfully");
    }

    FrameBuffer::FrameBuffer(unsigned int p_width, unsigned int p_height, FrameBufferMode in) :
        m_size{ p_width,p_height },
        m_width{ m_size.x },
        m_height{ m_size.y },
        FrameBufferType{ in }
    {
        if (p_width == 0 || p_height == 0)
        {
            ENGINE_LOG_ASSERT(false, "Width or Height cant be 0");
            std::exit(EXIT_FAILURE);
        }

        if (in == FrameBufferMode::FBM_NONE || in == FrameBufferMode::MAXCOUNT)
        {
            ENGINE_LOG_ASSERT(false, "Creating FrameBuffer with Invalid Type");
            std::exit(EXIT_FAILURE);
        }

        m_data.hiddentype = in;
        Name = getStringForEnum(static_cast<int>(in));
        CreateFrameBuffer(m_width, m_height);
        std::string SuccessMsg = "FrameBuffer [" + Name + "]  Created Successfully";
        ENGINE_CORE_INFO(SuccessMsg.c_str());
    }

    FrameBuffer::~FrameBuffer()
    {
    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);
        glViewport(0, 0, OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight());
    }

    void FrameBuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Clear() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Eclipse::FrameBuffer::Resize(unsigned width, unsigned height)
    {
        DeletCurrentFrameBuffer();
        Eclipse::OpenGL_Context::CreateFrameBuffers(width, height, FrameBufferMode::FBM_GAME);
        EDITOR_LOG_INFO("Resize Successful");
    }

    void FrameBuffer::ShowWindow(FrameBuffer g, const char* input)
    {
        (void)(input);

        if (&g == nullptr)
        {
            ENGINE_LOG_ASSERT(false, "FrameBuffer is Nullptr");
            std::exit(EXIT_FAILURE);
        }

        //  bind back to default framebuffer and draw with the attached framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, g.GetTextureColourBufferID());
    }

    void FrameBuffer::CreateFrameBuffer(unsigned int p_width, unsigned int p_height)
    {
        glGenFramebuffers(1, &m_data.frameBufferID);

        glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);

        glEnable(GL_DEPTH_TEST);

        glGenTextures(1, &m_data.TextureColourBuffer);
        glBindTexture(GL_TEXTURE_2D, m_data.TextureColourBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_data.TextureColourBuffer, 0);

        glGenRenderbuffers(1, &m_data.depthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_data.depthBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, p_width, p_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_data.depthBufferID);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << " Framebuffer Type : " << m_data.hiddentype << " is not complete!" << std::endl;
            ENGINE_LOG_ASSERT(false, " Framebuffer is not complete!");
            std::exit(EXIT_FAILURE);
        }

        Unbind();
    }

    void FrameBuffer::CreatePostProcessFramebuffer()
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

    void FrameBuffer::UpdatePP()
    {
        if (AllowPostProcess == false)
            return;

        if (PPType_ == PostProcessType::PPT_NONE)
            return;

        engine->MaterialManager.DoNotUpdateStencil();
        glBindFramebuffer(GL_FRAMEBUFFER, engine->GraphicsManager.GetFrameBufferID(FrameBufferMode::FBM_SCENE));

        auto& shdrpgm = Graphics::shaderpgms["PostProcess"];
        shdrpgm.Use();

        GLint Inversion = shdrpgm.GetLocation("Type");
        GLCall(glUniform1i(Inversion, static_cast<GLint>(PPType_)));

        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, engine->GraphicsManager.GetTextureID(FrameBufferMode::FBM_SCENE));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shdrpgm.UnUse();
    }

    std::string Eclipse::FrameBuffer::getStringForEnum(int enum_val)
    {
        std::string tmp(enum_FrameBufferMode_str[enum_val]);
        return tmp;
    }

    unsigned int FrameBuffer::GetFrameBufferID()
    {
        return m_data.frameBufferID;
    }

    unsigned int FrameBuffer::GetTextureColourBufferID()
    {
        return m_data.TextureColourBuffer;
    }

    unsigned int FrameBuffer::GetDepthBufferID()
    {
        return m_data.depthBufferID;
    }

    FrameBufferMode Eclipse::FrameBuffer::GetFrameBufferType()
    {
        return FrameBufferType;
    }

    std::string Eclipse::FrameBuffer::GetName()
    {
        return Name;
    }

    std::ostream& Eclipse::operator<<(std::ostream& os, const FrameBufferMode& in)
    {
        switch (in)
        {
        case FrameBufferMode::FBM_GAME:
            os << "Game FrameBuffer";
            break;

        case FrameBufferMode::FBM_SCENE:
            os << "Scene FrameBuffer";
            break;

        case FrameBufferMode::FBM_BOTTOM:
            os << "SWITCHINGVIEWS_BOTTOM FrameBuffer";
            break;

        case FrameBufferMode::FBM_LEFT:
            os << "SWITCHINGVIEWS_LEFT FrameBuffer";
            break;

        case FrameBufferMode::FBM_RIGHT:
            os << "SWITCHINGVIEWS_RIGHT FrameBuffer";
            break;

        case FrameBufferMode::FBM_TOP:
            os << "SWITCHINGVIEWS_TOP FrameBuffer";
            break;
        }

        return os;
    }

    void Eclipse::FrameBuffer::DeletCurrentFrameBuffer()
    {
        if (m_data.frameBufferID)
        {
            glDeleteFramebuffers(1, &m_data.frameBufferID);
            glDeleteTextures(1, &m_data.TextureColourBuffer);
            glDeleteTextures(1, &m_data.depthBufferID);

            engine->GraphicsManager.mRenderContext._Framebuffers.erase(FrameBufferType);
            EDITOR_LOG_INFO("FrameBuffer deleted successfully");
        }
    }

    void FrameBuffer::SetRenderMode(RenderMode _renderMode)
    {
        m_renderMode = _renderMode;
    }

    RenderMode FrameBuffer::GetRenderMode()
    {
        return m_renderMode;
    }
}