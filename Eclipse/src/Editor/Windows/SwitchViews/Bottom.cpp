#include "pch.h"
#include "Bottom.h"

namespace Eclipse
{
    void eBottomViewWindow::Update()
    {
        if (IsVisible)
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&eBottomViewWindow::InitilializeFrameBuffer, this));
    }

    eBottomViewWindow::eBottomViewWindow() :
        mViewportSize{ 0.0f, 0.0f }
    {
        Type = EditorWindowType::SWITCHVIEW_Bottom;
        WindowName = "BottomView";

        m_frameBuffer = std::make_shared<FrameBuffer>(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_BOTTOM));
    }

    void eBottomViewWindow::InitilializeFrameBuffer()
    {
        ImVec2 viewportPanelSize = ECGui::GetWindowSize();

        if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
        {
            // Resize the framebuffer based on the size of the imgui window
            //m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
            mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        }

        ChildSettings settings;
        settings.Name = "BottomView";
        settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
        ECGui::DrawChildWindow<void()>(settings, std::bind(&eBottomViewWindow::RunFrameBuffer, this));
    }

    void eBottomViewWindow::RunFrameBuffer()
    {
        // Set Image size
        ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
            ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (ECGui::IsItemHovered())
        {
            // Do all the future stuff here
        }
    }
}