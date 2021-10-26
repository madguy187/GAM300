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
			if (!strcmp(member.GetName(), "About Eclipse" ICON_MDI_CROSSHAIRS_QUESTION))
				ECGui::DrawMenuComponent<void()>(member.GetName(),
					std::bind(&MenuBar::ShowTeamInformation, this));
			else
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

			ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_NoFocusOnClick;
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

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

	void MenuBar::ShowTeamInformation()
	{
		ECGui::DrawTextWidget<const char*>("Team Unagi Studios", EMPTY_STRING);
		ECGui::InsertHorizontalLineSeperator();
		ECGui::DrawTextWidget<const char*>("[BAGD] " ICON_FA_BOOK, EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Producer/Systems Designer: Janelle Tan", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Design Lead/UI & UX Designer: Denny Liong", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Art Lead/Audio Lead/Level Design: Brina Chia", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("\n[BSGD/RTIS] " ICON_FA_STACK_OVERFLOW, EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Tech Lead/Architecture: Nico Wong", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Editor Lead/Technical Audio Designer: Fikrul Islami", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Physics/Collision Programmer: Guan Hin", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Serialization Programmer: Jian Herng", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Editor Programmer/Level Designer: Tian Yu", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Graphics Lead: Darren Sim", EMPTY_STRING);
		ECGui::DrawTextWidget<const char*>("Graphics Programmer: Rachel Lin", EMPTY_STRING);
		ECGui::InsertHorizontalLineSeperator();
		ECGui::DrawTextWidget<const char*>("All content copyrighted 2021 DigiPen (SINGAPORE) Corporation, all rights reserved.", EMPTY_STRING);
	}

	void MenuBar::ShowRecoveryDialogBox(bool active)
	{
		if (active)
		{
			ECGui::OpenPopup("Recover?");

			ImGui::GetMainViewport()->Flags |= ImGuiViewportFlags_NoFocusOnClick;
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			ImVec2 center = ECGui::GetMainViewport()->GetCenter();
			ECGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ECGui::BeginPopupModal("Recover?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ECGui::DrawTextWidget<const char*>("The engine crashed unexpectedly previously.\n     Do you wanna recover unsaved scene?", EMPTY_STRING);
				ECGui::InsertHorizontalLineSeperator();

				if (ECGui::ButtonBool("OK", ImVec2(120, 0)))
				{
					// Load and delete recovery file
					std::string path = std::filesystem::current_path().string() + "\\" + engine->szManager.GetBackUpPath();
					SceneManager::RegisterScene(path);
					SceneManager::LoadScene(RetrieveFilename(engine->szManager.GetBackUpPath()));
					std::filesystem::remove(std::filesystem::path(path));
					engine->editorManager->SetRecoveryFileExistence(false);
					ECGui::CloseCurrentPopup();
				}

				ECGui::InsertSameLine();

				if (ECGui::ButtonBool("Cancel", ImVec2(120, 0)))
				{
					// Delete recovery file
					std::string path = std::filesystem::current_path().string() + "\\" + engine->szManager.GetBackUpPath();
					std::filesystem::remove(std::filesystem::path(path));
					engine->editorManager->SetRecoveryFileExistence(false);
					ECGui::CloseCurrentPopup();
				}

				ECGui::EndPopup();
			}
		}
	}

	std::string MenuBar::RetrieveFilename(std::string path)
	{
		size_t pos = path.find('.');

		if (pos == std::string::npos)
			return std::string();

		pos = path.find_last_of('\\');
		std::string temp = path.substr(pos + 1);

		temp.erase(temp.find('.'));

		return temp;
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
