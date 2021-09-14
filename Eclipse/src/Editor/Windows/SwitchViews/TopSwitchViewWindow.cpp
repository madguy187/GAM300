#include "pch.h"
#include "TopSwitchViewWindow.h"

namespace Eclipse
{
	void TopSwitchViewWindow::Update()
	{
		BaseSwitchViewWindow::Update();
	}

	void TopSwitchViewWindow::Init()
	{
		mViewportSize = glm::vec2{ 0.0f, 0.0f };
		Type = EditorWindowType::EWT_SWITCHVIEW_TOP;
		WindowName = "Top Switch View";
		IsVisible = false;
		m_frameBuffer = engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_TOP);
		mProjectionView_List.push_back("Orthographic");
	}

	void TopSwitchViewWindow::Unload()
	{
		BaseSwitchViewWindow::Unload();
	}
}
