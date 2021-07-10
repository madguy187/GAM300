#include "pch.h"
#include "GameView.h"

namespace Eclipse
{
	void eGameViewWindow::Update()
	{
		//Profiler timer;
		//timer.SetName({ SystemName::TEST });
		//timer.tracker.system_start = glfwGetTime();
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&eGameViewWindow::InitilializeFrameBuffer, this));
		//timer.tracker.system_end = glfwGetTime();
		//
		//timer.ContainerAddTime(timer.tracker);
	}

	eGameViewWindow::eGameViewWindow() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::EWT_GAMEVIEW;
		WindowName = "Game View";

		m_frameBuffer = std::make_shared<FrameBuffer>(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::GAMEVIEW));
	}

	void eGameViewWindow::InitilializeFrameBuffer()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "GameViewFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&eGameViewWindow::RunFrameBuffer, this));
	}

	void eGameViewWindow::RunFrameBuffer()
	{
		// Set Image size
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())), 
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
		}
	}
}