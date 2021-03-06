#include "pch.h"
#include "RightSwitchViewWindow.h"

namespace Eclipse
{
	void RightSwitchViewWindow::Update()
	{
		BaseSwitchViewWindow::Update();
	}

	void RightSwitchViewWindow::Init()
	{
		FBO_Mode = FrameBufferMode::FBM_RIGHT;
		mCamType = CameraComponent::CameraType::RightView_camera;
		mViewportSize = glm::vec2{ 0.0f, 0.0f };
		Type = EditorWindowType::EWT_SWITCHVIEW_RIGHT;
		WindowName = "Right Switch View " ICON_MDI_MONITOR;
		IsVisible = false;
		m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_RIGHT);
		mProjectionView_List.push_back("Orthographic");
	}

	void RightSwitchViewWindow::Unload()
	{
		BaseSwitchViewWindow::Unload();
	}
}
