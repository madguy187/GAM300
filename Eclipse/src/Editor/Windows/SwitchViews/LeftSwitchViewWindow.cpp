#include "pch.h"
#include "LeftSwitchViewWindow.h"

namespace Eclipse
{
	void LeftSwitchViewWindow::Update()
	{
		BaseSwitchViewWindow::Update();
	}

	void LeftSwitchViewWindow::Init()
	{
		mCamType = CameraComponent::CameraType::LeftView_Camera;
		mViewportSize = glm::vec2{ 0.0f, 0.0f };
		Type = EditorWindowType::EWT_SWITCHVIEW_LEFT;
		WindowName = "Left Switch View " ICON_MDI_MONITOR;
		IsVisible = false;
		m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_LEFT);
		mProjectionView_List.push_back("Orthographic");
	}

	void LeftSwitchViewWindow::Unload()
	{
		BaseSwitchViewWindow::Unload();
	}
}
