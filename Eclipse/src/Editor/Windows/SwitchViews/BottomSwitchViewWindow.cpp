#include "pch.h"
#include "BottomSwitchViewWindow.h"

namespace Eclipse
{
    void BottomSwitchViewWindow::Update()
    {
        BaseSwitchViewWindow::Update();
    }

    void BottomSwitchViewWindow::Init()
    {
        mCamType = CameraComponent::CameraType::BottomView_Camera;
        mViewportSize = glm::vec2{ 0.0f, 0.0f };
        Type = EditorWindowType::EWT_SWITCHVIEW_BOTTOM;
        WindowName = "Bottom Switch View";
        IsVisible = false;
        m_frameBuffer = engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::FBM_BOTTOM);
        mProjectionView_List.push_back("Orthographic");
    }

    void BottomSwitchViewWindow::Unload()
    {
        BaseSwitchViewWindow::Unload();
    }
}
