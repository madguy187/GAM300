#include "pch.h"
#include "MenuComponent.h"

#include "Editor/Utilities/OpenFileDialog/OpenFileDialog.h"
#include "Editor/Windows/Scene/SceneView.h"
#include "Editor/Windows/GameView/GameView.h"

namespace Eclipse
{
	void MenuComponent::Update()
	{
		for (const auto& str : List_)
		{
			DrawImpl(str.c_str());
		}

		DrawGuiWindows();
	}

	MenuComponent::MenuComponent(const char* name, EditorMenuType type) :
		Name_{ name }, Type_{ type }, ID{ 0 } {}

	void MenuComponent::AddItems(const char* name)
	{
		List_.push_back(std::string{ name });
		ListToName_[ID] = name;
		ID++;
	}

	void MenuComponent::DrawImpl(const char* key)
	{
		// For specific items
		if (!strcmp(key, "New " ICON_MDI_FOLDER_PLUS))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				if (!engine->GetPlayState())
				{
					SceneManager::NewScene();
				}
			}
		}
		
		if (!strcmp(key, "Open " ICON_MDI_FOLDER_OPEN))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected, "CTRL+O"))
			{
				if (!engine->GetPlayState())
				{
					FileDialog::FileBrowser();
				}
			}
		
		
		}

		if (!strcmp(key, "Save " ICON_MDI_CONTENT_SAVE))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected, "CTRL+S"))
			{
				if (!engine->GetPlayState())
				{
					FileDialog::SaveFile();
				}
			}
		}

		if (!strcmp(key, "Save As... " ICON_MDI_CONTENT_SAVE_EDIT))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				if (!engine->GetPlayState())
				{
					FileDialog::SaveAsFile();
				}
			}
		}

		if (!strcmp(key, "Exit " ICON_MDI_EXIT_TO_APP))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				engine->editorManager->GetMenuBar().SetExitStatus(true);
			}
		}
		
		if (!strcmp(key, "Undo " ICON_MDI_UNDO_VARIANT))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected, "CTRL+Z"))
			{
				CommandHistory::Undo();
			}
		}

		if (!strcmp(key, "Redo " ICON_MDI_REDO_VARIANT))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected, "CTRL+Y"))
			{
				CommandHistory::Redo();
			}
		}

		if (!strcmp(key, "Oppa GuanHin Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsLight();
			}
		}

		if (!strcmp(key, "Oppa Nico Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsWooden();
			}
		}

		if (!strcmp(key, "Oppa Fikrul Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsCrimson();
			}
		}

		if (!strcmp(key, "Oppa TianYu Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsCherry();
			}
		}

		if (!strcmp(key, "Oppa Janelle Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsDark();
			}
		}

		if (!strcmp(key, "Oppa Denny Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsArmy();
			}
		}

		if (!strcmp(key, "Oppa Brina Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsGoldBlack();
			}
		}


		if (!strcmp(key, "Oppa Rachel Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsRobotBlue();
			}
		}

		if (!strcmp(key, "Oppa Darren Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsClassic();
			}
		}

		if (!strcmp(key, "Oppa JianHerng Style"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				ImGui::StyleColorsArmy();
			}
		}
	}

	void MenuComponent::DrawGuiWindows()
	{
		if (!strcmp(Name_, "Windows" ICON_MDI_MONITOR))
		{
			int index = 0;

			for (auto& window : engine->editorManager->GetAllWindowsByRef())
			{
				if (!strcmp(ListToName_[index], "Mesh Editor"))
				{
					index++;
					continue;
				}

				if (ECGui::CreateMenuItem(ListToName_[index], &window->IsVisible))
				{
					if (window->IsVisible)
						window->IsVisible = true;
					else
						window->IsVisible = false;
				}

				index++;
			}
		}
	}

	EditorMenuType MenuComponent::GetType()
	{
		return Type_;
	}

	const char* MenuComponent::GetName()
	{
		return Name_;
	}
}