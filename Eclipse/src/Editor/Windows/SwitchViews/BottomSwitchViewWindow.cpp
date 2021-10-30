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
        FBO_Mode = FrameBufferMode::FBM_BOTTOM;
        mCamType = CameraComponent::CameraType::BottomView_Camera;
        mViewportSize = glm::vec2{ 0.0f, 0.0f };
        Type = EditorWindowType::EWT_SWITCHVIEW_BOTTOM;
        WindowName = "Bottom Switch View " ICON_MDI_MONITOR;
        IsVisible = false;
        m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_BOTTOM);
        mProjectionView_List.push_back("Orthographic");
    }

    void BottomSwitchViewWindow::Unload()
    {
        BaseSwitchViewWindow::Unload();
    }
}
