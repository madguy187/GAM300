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
        //ENGINE_CORE_INFO("FrameBuffer Created Successfully");
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

        if (in == FrameBufferMode::FBM_SHADOW)
        {
            CreateShadowFBO(m_width, m_height);
        }
        else
        {
            CreateFrameBuffer(m_width, m_height);
        }

        //std::string SuccessMsg = "FrameBuffer [" + Name + "]  Created Successfully";
        //ENGINE_CORE_INFO(SuccessMsg.c_str());
    }

    FrameBuffer::~FrameBuffer()
    {

    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);
    }

    void FrameBuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::SetViewPort() const
    {
        glViewport(0, 0, m_width, m_height);
    }

    void FrameBuffer::Clear() const
    {
        glViewport(0, 0, m_width, m_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Eclipse::FrameBuffer::Resize(unsigned width, unsigned height, FrameBufferMode Mode)
    {
        engine->gFrameBufferManager->CreateFBO(width, height, Mode);

        engine->gFrameBufferManager->FrameBufferContainer[Mode]->m_height = height;
        engine->gFrameBufferManager->FrameBufferContainer[Mode]->m_width = width;

        //EDITOR_LOG_INFO("Resize Successful");
    }

    void FrameBuffer::ShowWindow(FrameBuffer g)
    {
        if (&g == nullptr)
        {
            ENGINE_LOG_ASSERT(false, "FrameBuffer is Nullptr");
            std::exit(EXIT_FAILURE);
        }

        //  bind back to default framebuffer and draw with the attached framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, g.GetTextureColourBufferID());
    }

    void FrameBuffer::DeleteBuffers()
    {
        if (m_data.frameBufferID)
        {
            glDeleteFramebuffers(GL_FRAMEBUFFER, &m_data.frameBufferID);
            glDeleteTextures(1, &m_data.ColorBuffers[0]);
            glDeleteTextures(1, &m_data.ColorBuffers[1]);
            glDeleteTextures(1, &m_data.depthBufferID);
            m_data.ColorBuffers[0] = 0;
            m_data.ColorBuffers[1] = 0;
            m_data.depthBufferID = 0;

            engine->gFrameBufferManager->DeleteFrameBuffer(FrameBufferType);
        }

    }

    void FrameBuffer::CreateShadowFBO(unsigned int p_width, unsigned int p_height)
    {
        m_width = p_width;
        m_height = p_height;

        glGenFramebuffers(1, &m_data.frameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);

        glGenTextures(1, &m_data.ColorBuffers[0]);
        glBindTexture(GL_TEXTURE_2D, m_data.ColorBuffers[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_data.ColorBuffers[0], 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::CreateFrameBuffer(unsigned int p_width, unsigned int p_height)
    {
        m_width = p_width;
        m_height = p_height;

        if (m_data.frameBufferID)
        {
            DeleteBuffers();
        }

        glGenFramebuffers(1, &m_data.frameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);

        glGenTextures(2, m_data.ColorBuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindTexture(GL_TEXTURE_2D, m_data.ColorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, p_width, p_height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_data.ColorBuffers[i], 0);
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_data.depthBufferID);
        glBindTexture(GL_TEXTURE_2D, m_data.depthBufferID);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, p_width, p_height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_data.depthBufferID, 0);

        GLenum buffers[2] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, buffers);

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
        return m_data.ColorBuffers[0];
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
        case FrameBufferMode::FBM_MATERIALEDITOR:
            os << "Material_EDITOR FrameBuffer";
            break;

        case FrameBufferMode::FBM_MESHEDITOR:
            os << "MESH_EDITOR FrameBuffer";
            break;

        case FrameBufferMode::FBM_GAME_SOBEL:
            os << "Game View For Sobel";
            break;
        }

        return os;
    }

    void Eclipse::FrameBuffer::DeletCurrentFrameBuffer()
    {
        if (m_data.frameBufferID)
        {
            glDeleteFramebuffers(1, &m_data.frameBufferID);
            glDeleteTextures(1, &m_data.ColorBuffers[0]);
            glDeleteTextures(1, &m_data.ColorBuffers[1]);
            glDeleteTextures(1, &m_data.depthBufferID);

            //engine->GraphicsManager.mRenderContext._Framebuffers.erase(FrameBufferType);
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