#include "pch.h"
#include "BottomSwitchViewWindow.h"

namespace Eclipse
{
	void BottomSwitchViewWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&BottomSwitchViewWindow::RunMainWindow, this));
	}

	void BottomSwitchViewWindow::Unload()
	{
	}

	BottomSwitchViewWindow::BottomSwitchViewWindow() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::EWT_SWITCHVIEW_BOTTOM;
		WindowName = "Bottom Switch View";
		IsVisible = false;

		m_frameBuffer_BOTTOM = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_BOTTOM));
		//engine->gCamera.GetCameraID(CameraComponent::CameraType::);
	}

	void BottomSwitchViewWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "BottomSwitchViewFrameBuffers";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&BottomSwitchViewWindow::RunFrameBuffer, this));
	}

	void BottomSwitchViewWindow::RunFrameBuffer()
	{
		// BOTTOM LEFT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_BOTTOM->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
		}
	}
}
