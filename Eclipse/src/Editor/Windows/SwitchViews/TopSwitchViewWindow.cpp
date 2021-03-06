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
        FBO_Mode = FrameBufferMode::FBM_TOP;
        mCamType = CameraComponent::CameraType::TopView_Camera;
        mViewportSize = glm::vec2{ 0.0f, 0.0f };
        Type = EditorWindowType::EWT_SWITCHVIEW_TOP;
        WindowName = "Top Switch View " ICON_MDI_MONITOR;
        IsVisible = false;
        m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_TOP);
        mProjectionView_List.push_back("Orthographic");
    }

    void TopSwitchViewWindow::Unload()
    {
        BaseSwitchViewWindow::Unload();
    }
}
