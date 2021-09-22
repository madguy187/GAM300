#include "pch.h"
#include "HeaderWindow.h"

namespace Eclipse
{
	void HeaderWindow::Update()
	{
		HideTabBar(IsTabBarHidden);

		if (ECGui::BeginMainWindowWithFlag(WindowName), NULL,
			/*ImGuiWindowFlags_NoTitleBar |*/ ImGuiWindowFlags_NoScrollbar)
		{
			RunPlayPauseStep();
		}

		ECGui::EndMainWindow();
	}

	void HeaderWindow::Init()
	{
		Type = EditorWindowType::EWT_HEADER;
		WindowName = "Header";
		IsVisible = true;
	}

	void HeaderWindow::Unload()
	{}

	void HeaderWindow::RunPlayPauseStep()
	{
		ECGui::InsertSameLine(ECGui::GetWindowSize().x / 2.25f);
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
				ImGui::SetWindowFocus("Game View");
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

	void HeaderWindow::HideTabBar(bool active)
	{
		if (!active)
		{
			ImGuiWindowClass windowClass;
			windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
			ImGui::SetNextWindowClass(&windowClass);
			IsTabBarHidden = true;
		}
	}
}