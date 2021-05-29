#include "pch.h"
#include "FrameBuffer.h"
#include "WindowConstraint.h"
#include "../src/ECS/World.h"


FrameBuffer::FrameBuffer(const glm::uvec2& p_size) :
  m_size{ p_size }, m_width{ m_size.x }, m_height{ m_size.y }
{
  CreateFrameBuffer(m_width, m_height);

}

FrameBuffer::FrameBuffer(unsigned int p_width, unsigned int p_height) :
  m_size{ p_width,p_height }, m_width{ m_size.x }, m_height{ m_size.y }
{
  CreateFrameBuffer(m_width, m_height);
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);
  glViewport(0, 0, OpenGL_Context::width, OpenGL_Context::height);
}

void FrameBuffer::Unbind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Clear() const
{
  glClearColor(0.1f, 0.2f, 0.3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::Init()
{

}

void FrameBuffer::Update()
{
}

void FrameBuffer::ShowWindow(FrameBuffer g)
{
  g.GetTextureColourBuffer();

  //  bind back to default framebuffer and draw with the attached framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, g.GetTextureColourBuffer());

  ImGui::Begin("Scene View");

  ImVec2 pos = ImGui::GetCursorScreenPos();

  ImGui::GetWindowDrawList()->AddImage(
    (void*)(g.m_data.TextureColourBuffer),
    ImVec2(ImGui::GetCursorScreenPos()),
    ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionMax().x,
      ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionMax().y), ImVec2(0, 1), ImVec2(1, 0));

  g.m_width = ImGui::GetWindowWidth();
  g.m_height = ImGui::GetWindowHeight();

  g.windowPos.x = (ImGui::GetWindowWidth() / 2) + ImGui::GetCursorScreenPos().x;
  g.windowPos.y = (ImGui::GetWindowHeight() / 2) + ImGui::GetCursorScreenPos().y;

  g.frameBufferPosition = vec2{ ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };
  ImGui::End();
}

void FrameBuffer::CreateFrameBuffer(unsigned int p_width, unsigned int p_height)
{
  glGenFramebuffers(1, &m_data.frameBufferID);
  glGenRenderbuffers(1, &m_data.depthBufferID);

  glBindRenderbuffer(GL_RENDERBUFFER, m_data.depthBufferID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, OpenGL_Context::width, OpenGL_Context::height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_data.depthBufferID);

  glBindFramebuffer(GL_FRAMEBUFFER, m_data.frameBufferID);
  glGenTextures(1, &m_data.TextureColourBuffer);
  glBindTexture(GL_TEXTURE_2D, m_data.TextureColourBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_width, p_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_data.TextureColourBuffer, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  }

  Unbind();
}

unsigned int FrameBuffer::GetGameViewBuffer()
{
  return m_data.frameBufferID;
}

unsigned int FrameBuffer::GetTextureColourBuffer()
{
  return m_data.TextureColourBuffer;
}
