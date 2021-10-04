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
			ECGui::OpenPopup("Exit?");

			ImVec2 center = ECGui::GetMainViewport()->GetCenter();
			ECGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ECGui::BeginPopupModal("Exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ECGui::DrawTextWidget<const char*>("Are you sure you want to exit the engine?\nAll unsaved work cannot be recovered.\n\n", EMPTY_STRING);
				ECGui::InsertHorizontalLineSeperator();

				if (ECGui::ButtonBool("OK", ImVec2(120, 0)))
				{
					EDITOR_LOG_INFO("Exiting application...");
					glfwSetWindowShouldClose(OpenGL_Context::GetWindow(), 1);
				}

				ECGui::InsertSameLine();

				if (ECGui::ButtonBool("Cancel", ImVec2(120, 0)))
				{
					IsExiting = false;
					ECGui::CloseCurrentPopup();
				}

				ECGui::EndPopup();
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
