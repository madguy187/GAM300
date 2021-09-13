#include "pch.h"
#include "MenuBar.h"

namespace Eclipse
{
	void MenuBar::Update()
	{
		ECGui::DrawMenuBar<void()>(std::bind(&MenuBar::DrawImpl, this));
	}

	void MenuBar::DrawImpl()
	{
		for (auto& member : MenuComponents_)
		{
			ECGui::DrawMenuComponent<void()>(member.GetName(),
											 [&]() { member.Update(); });
		}

		ShowExitDialogBox(IsExiting);
	}

	void MenuBar::AddMenuComponents(MenuComponent com)
	{
		MenuComponents_.push_back(com);
	}

	MenuComponent* MenuBar::GetMenuComponent(EditorMenuType type)
	{
		for (auto& member : MenuComponents_)
		{
			if (member.GetType() == type)
				return &member;
		}

		return nullptr;
	}

	void MenuBar::ShowExitDialogBox(bool active)
	{
		if (active)
		{
			ImGui::OpenPopup("Exit?");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Are you sure you want to exit the engine?\nAll unsaved work cannot be recovered.\n\n");
				ImGui::Separator();

				if (ImGui::Button("OK", ImVec2(120, 0)))
				{
					EDITOR_LOG_INFO("Exiting application...");
					glfwSetWindowShouldClose(OpenGL_Context::GetWindow(), 1);
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					IsExiting = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
	}

	bool MenuBar::GetExitStatus()
	{
		return IsExiting;
	}

	void MenuBar::SetExitStatus(bool check)
	{
		IsExiting = check;
	}
}
