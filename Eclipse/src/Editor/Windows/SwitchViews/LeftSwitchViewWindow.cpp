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
		mViewportSize = glm::vec2{ 0.0f, 0.0f };
		Type = EditorWindowType::EWT_SWITCHVIEW_LEFT;
		WindowName = "Left Switch View";
		IsVisible = false;
		m_frameBuffer = engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_LEFT);
		mProjectionView_List.push_back("Orthographic");
	}

	void LeftSwitchViewWindow::Unload()
	{
		BaseSwitchViewWindow::Unload();
	}
}
