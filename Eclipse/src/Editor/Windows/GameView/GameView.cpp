#include "pch.h"
#include "GameView.h"
#include "imgui_internal.h"

namespace Eclipse
{
    void eGameViewWindow::Update()
    {
        if (IsVisible)
        {
            if (IsViewFullscreen && !IsViewBackToOriginalState)
            {
                SetFullScreenViewport();
            }
            else if (IsViewBackToOriginalState && !IsViewFullscreen)
            {
                ECGui::SetNextWindowDockID(DockIDTracker);
                AlreadySetDockIDTracker = false;
            }

            IsWindowRunning = ECGui::DrawMainWindow<void()>(WindowName, std::bind(&eGameViewWindow::RunMainWindow, this));
        }
        else
            IsWindowRunning = false;
    }

    void eGameViewWindow::Init()
    {
        mViewportSize = ECVec2{ 0.0f, 0.0f };
        Type = EditorWindowType::EWT_GAMEVIEW;
        WindowName = "Game View " ICON_MDI_MONITOR;
        m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_GAME);
    }

    void eGameViewWindow::Unload()
    {
    }

    void eGameViewWindow::RunMainWindow()
    {
        ImVec2 viewportPanelSize = ECGui::GetWindowSize();

        if (!AlreadySetDockIDTracker)
            DockIDTracker = ImGui::GetWindowDockID();

        if (mViewportSize.getX() != viewportPanelSize.x ||
            mViewportSize.getY() != viewportPanelSize.y)
        {
            // Resize the framebuffer based on the size of the imgui window
            //m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
            m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y), FrameBufferMode::FBM_GAME);
            mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            engine->gFrameBufferManager->UpdateAspectRatio(FrameBufferMode::FBM_GAME, mViewportSize);

            // Darren Was here , i need to treat this new FBO same as GAMEVIEW 
            //engine->gFrameBufferManager->FrameBufferContainer[FrameBufferMode::FBM_GAME_SOBEL]->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y), FrameBufferMode::FBM_GAME_SOBEL);
            //engine->gFrameBufferManager->UpdateAspectRatio(FrameBufferMode::FBM_GAME_SOBEL, mViewportSize);
        }

        ChildSettings settings;
        settings.Name = "GameViewFrameBuffer";
        settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
        ECGui::DrawChildWindow<void()>(settings, std::bind(&eGameViewWindow::RunFrameBuffer, this));
    }

    void eGameViewWindow::RunFrameBuffer()
    {
        // Darren Was Here.
        vMin = ImGui::GetWindowContentRegionMin();
        vMax = ImGui::GetWindowContentRegionMax();

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;
        //
         
        //RenderGameHeader();
        // Set Image size
        ECGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
            ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (ECGui::IsItemHovered())
        {
            // Do all the future stuff here
            // marry anna kendrick
        }
    }

    void eGameViewWindow::SetFullScreenViewport()
    {
        RECT desktop;
        // Get a handle to the desktop window
        const HWND hDesktop = GetDesktopWindow();
        // Get the size of screen to the variable desktop
        GetWindowRect(hDesktop, &desktop);
        ECGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiCond_Always);
        ECGui::SetNextWindowSize(ImVec2((float)(desktop.right), (float)(desktop.bottom)),
            ImGuiCond_Always);
        AlreadySetDockIDTracker = true;
    }

    bool eGameViewWindow::GetIsViewFullscreen()
    {
        return IsViewFullscreen;
    }

    void eGameViewWindow::SetViewToFullscreen(bool active)
    {
        IsViewFullscreen = active;
    }

    void eGameViewWindow::SetViewToOriginalState(bool active)
    {
        IsViewBackToOriginalState = active;
    }

    ECVec2 eGameViewWindow::GetViewPortSize()
    {
        return mViewportSize;
    }

    bool eGameViewWindow::GetIsWindowRunning() const
    {
        return IsWindowRunning;
    }
}
