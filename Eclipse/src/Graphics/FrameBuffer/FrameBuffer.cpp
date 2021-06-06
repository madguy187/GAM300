#include "pch.h"
#include "FrameBuffer.h"
#include "WindowConstraint.h"
#include "../src/ECS/World.h"
#include "EntryPoint/EntryPoint.h"

FrameBuffer::FrameBuffer(const glm::uvec2& p_size, FrameBufferMode in) :
    m_size{ p_size },
    m_width{ m_size.x },
    m_height{ m_size.y },
    FrameBufferType{ in }
{
    m_data.hiddentype = in;
    CreateFrameBuffer(m_width, m_height);

}

FrameBuffer::FrameBuffer(unsigned int p_width, unsigned int p_height, FrameBufferMode in) :
    m_size{ p_width,p_height },
    m_width{ m_size.x },
    m_height{ m_size.y },
    FrameBufferType{ in }
{
    if (p_width == 0 || p_height == 0)
    {
        ENGINE_CORE_INFO("Width or Height cant be 0");
        ENGINE_LOG_ASSERT(false, "Width or Height cant be 0");
        std::exit(EXIT_FAILURE);
    }

    if (in == Eclipse::FrameBufferMode::NONE || in == Eclipse::FrameBufferMode::MAXCOUNT)
    {
        ENGINE_CORE_INFO("Creating FrameBuffer with Invalid Type");
        ENGINE_LOG_ASSERT(false, "Creating FrameBuffer with Invalid Type");
        std::exit(EXIT_FAILURE);
    }

    m_data.hiddentype = in;
    CreateFrameBuffer(m_width, m_height);
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);
    glViewport(0, 0, OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Clear() const
{
    //glClearColor(0.1f, 0.2f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Eclipse::FrameBuffer::Resize(unsigned width, unsigned height)
{
    if (m_data.frameBufferID)
    {
        glDeleteFramebuffers(1, &m_data.frameBufferID);
        glDeleteTextures(1, &m_data.TextureColourBuffer);
        glDeleteTextures(1, &m_data.depthBufferID);
    }

    Eclipse::OpenGL_Context::CreateFrameBuffers(OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight(), Eclipse::FrameBufferMode::SCENEVIEW);
}

void FrameBuffer::ShowWindow(FrameBuffer g, const char* input)
{
    if (&g == nullptr)
    {
        ENGINE_CORE_INFO("FrameBuffer is Nullptr");
        ENGINE_LOG_ASSERT(false, "FrameBuffer is Nullptr");
        std::exit(EXIT_FAILURE);
    }

    //  bind back to default framebuffer and draw with the attached framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, g.GetTextureColourBufferID());
}

void FrameBuffer::CreateFrameBuffer(unsigned int p_width, unsigned int p_height)
{
    glGenFramebuffers(1, &m_data.frameBufferID);

    glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);

    glEnable(GL_DEPTH_TEST);

    glGenTextures(1, &m_data.TextureColourBuffer);
    glBindTexture(GL_TEXTURE_2D, m_data.TextureColourBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_width, p_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_data.TextureColourBuffer, 0);

    glGenRenderbuffers(1, &m_data.depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_data.depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, OpenGL_Context::GetWidth(), OpenGL_Context::GetHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_data.depthBufferID);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << " Framebuffer Type : " << m_data.hiddentype << " is not complete!" << std::endl;
        ENGINE_CORE_INFO(" Framebuffer is not complete!");
        ENGINE_LOG_ASSERT(false, " Framebuffer is not complete!");
        std::exit(EXIT_FAILURE);
    }

    Unbind();
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

std::ostream& Eclipse::operator<<(std::ostream& os, const FrameBufferMode& in)
{
    switch (in)
    {
    case FrameBufferMode::GAMEVIEW:
        os << "Game FrameBuffer";
        break;

    case FrameBufferMode::SCENEVIEW:
        os << "Scene FrameBuffer";
        break;
    }

    return os;
}
