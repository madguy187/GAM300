#include "pch.h"
#include "GameView.h"

namespace Eclipse
{
	void eGameViewWindow::Update()
	{
		//Profiler timer;
		//timer.SetName({ SystemName::TEST });
		//timer.tracker.system_start = glfwGetTime();
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&eGameViewWindow::RunMainWindow, this));
		//timer.tracker.system_end = glfwGetTime();
		//
		//timer.ContainerAddTime(timer.tracker);
	}

	void eGameViewWindow::Init()
	{
		mViewportSize = glm::vec2{ 0.0f, 0.0f };
		Type = EditorWindowType::EWT_GAMEVIEW;
		WindowName = "Game View";
		m_frameBuffer = std::make_shared<FrameBuffer>(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::FBM_GAME));
	}

	void eGameViewWindow::Unload()
	{
	}

	void eGameViewWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "GameViewFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&eGameViewWindow::RunFrameBuffer, this));
	}

	void eGameViewWindow::RunFrameBuffer()
	{
		//RenderGameHeader();
		// Set Image size
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())), 
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
			// marry anna kendrick
		}
	}

	void eGameViewWindow::RenderGameHeader()
	{
		ECGui::InsertSameLine(mViewportSize.x / 2.35f);
		if (ECGui::ButtonBool("Play " ICON_FA_PLAY))
		{
			if (engine->GetPlayState())
			{
				engine->SetPlayState(false);
				engine->SetPauseState(false);
				ImGui::SetWindowFocus("Scene View");
				EDITOR_LOG_INFO("Scene has stopped playing. Reverting to original state...");
			}
			else
			{
				engine->SetPlayState(true);
				EDITOR_LOG_INFO("Scene is playing...");
			}
		}

		ECGui::InsertSameLine();
		if (ECGui::ButtonBool("Pause " ICON_FA_PAUSE))
		{
			if (engine->GetPlayState())
			{
				if (engine->GetPauseState())
				{
					engine->SetPauseState(false);
					EDITOR_LOG_INFO("Scene is unpaused!");
				}
				else
				{
					engine->SetPauseState(true);
					EDITOR_LOG_INFO("Scene is paused!");
				}
			}
			else
			{
				EDITOR_LOG_WARN("Scene has to be playing first.");
			}
		}

		ECGui::InsertSameLine();
		if (ECGui::ButtonBool("Step " ICON_FA_STEP_FORWARD))
		{
			if (engine->GetPlayState())
			{
				engine->SetStepState(true);
				EDITOR_LOG_INFO("Stepped scene.");
			}
			else
			{
				EDITOR_LOG_WARN("Scene has to be playing first.");
			}
		}
	}
}
