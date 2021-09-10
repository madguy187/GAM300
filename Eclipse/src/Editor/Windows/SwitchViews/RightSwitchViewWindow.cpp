#include "pch.h"
#include "RightSwitchViewWindow.h"

namespace Eclipse
{
	void RightSwitchViewWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&RightSwitchViewWindow::RunMainWindow, this));
	}

	void RightSwitchViewWindow::Unload()
	{
	}

	RightSwitchViewWindow::RightSwitchViewWindow() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::EWT_SWITCHVIEW_RIGHT;
		WindowName = "Right Switch View";
		IsVisible = false;

		m_frameBuffer_RIGHT = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_RIGHT));
		//engine->gCamera.GetCameraID(CameraComponent::CameraType::);
	}

	void RightSwitchViewWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "RightSwitchViewFrameBuffers";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&RightSwitchViewWindow::RunFrameBuffer, this));
	}

	void RightSwitchViewWindow::RunFrameBuffer()
	{
		// BOTTOM LEFT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_RIGHT->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
		}
	}
}
