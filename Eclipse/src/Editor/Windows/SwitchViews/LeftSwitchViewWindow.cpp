#include "pch.h"
#include "LeftSwitchViewWindow.h"

namespace Eclipse
{
	void LeftSwitchViewWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&LeftSwitchViewWindow::RunMainWindow, this));
	}

	void LeftSwitchViewWindow::Unload()
	{
	}

	LeftSwitchViewWindow::LeftSwitchViewWindow() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::EWT_SWITCHVIEW_LEFT;
		WindowName = "Left Switch View";
		IsVisible = false;

		m_frameBuffer_LEFT = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_LEFT));
		//engine->gCamera.GetCameraID(CameraComponent::CameraType::);
	}

	void LeftSwitchViewWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "LeftSwitchViewFrameBuffers";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&LeftSwitchViewWindow::RunFrameBuffer, this));
	}

	void LeftSwitchViewWindow::RunFrameBuffer()
	{
		// BOTTOM LEFT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_LEFT->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
		}
	}
}
