#include "pch.h"
#include "Graphics/RendererAPI/GraphicsManager.h"

void Eclipse::GraphicsManager::pre_render()
{  // Loading Configuration
  mRenderContext.init("../Dep/Configuration/configuration.json");

  // Loading Of Models , Shaders and etc.. 
  Graphics::load();

  // To be Removed
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplGlfw_InitForOpenGL(mRenderContext.ptr_window, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsClassic();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Clear the View
  mRenderContext.pre_render();
}

void Eclipse::GraphicsManager::post_render()
{
  mRenderContext.post_render();
}

void Eclipse::GraphicsManager::end()
{
  mRenderContext.end();
}

void Eclipse::GraphicsManager::unload()
{
  mRenderContext.end();
  ImGui::DestroyContext();
}

void Eclipse::GraphicsManager::draw()
{
  FrameBuffer::ShowWindow(*mRenderContext.m_frameBuffer, "GameView");
  FrameBuffer::ShowWindow(*mRenderContext.n_frameBuffer, "SceneView");
}

void Eclipse::GraphicsManager::ImguiRender()
{
  // To be Removed
  int Width, Height;
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwGetFramebufferSize(mRenderContext.ptr_window, &Width, &Height);
  mRenderContext.SetViewport(0, 0, Width, Height);
  mRenderContext.SetClearColor({ 0.1f, 0.2f, 0.3f, 1.f });
}

void Eclipse::GraphicsManager::UpdateFrameBuffer()
{
  mRenderContext.m_frameBuffer->Bind();
  mRenderContext.m_frameBuffer->Clear();

  mRenderContext.n_frameBuffer->Bind();
  mRenderContext.n_frameBuffer->Clear();
}

