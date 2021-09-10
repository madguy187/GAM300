#include "pch.h"
#include "MenuComponent.h"

#include "Editor/Utilities/OpenFileDialog.h"
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
		if (!strcmp(key, "New"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				SceneManager::NewScene();
			}
		}
		
		if (!strcmp(key, "Open"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				FileDialog::FileBrowser();
			}
		}

		if (!strcmp(key, "Save As..."))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				FileDialog::SaveFile();
			}
		}

		if (!strcmp(key, "Exit"))
		{
			bool selected = false;

			if (ECGui::CreateMenuItem(key, &selected))
			{
				glfwSetWindowShouldClose(OpenGL_Context::GetWindow(), 1);
			}
		}
		/*if (!strcmp(key, "Scene"))
		{
			auto* scene = engine->editorManager->GetEditorWindow<Scene>();

			if (ECGui::CreateMenuItem(key, &scene->IsVisible))
			{
				if (scene->IsVisible)
					scene->IsVisible = true;
				else
					scene->IsVisible = false;
			}
		}
		else if (!strcmp(key, "GameView"))
		{
			auto* game = engine->editorManager->GetEditorWindow<eGameView>();

			if (ECGui::CreateMenuItem(key, &game->IsVisible))
			{
				if (game->IsVisible)
					game->IsVisible = true;
				else
					game->IsVisible = false;
			}
		}*/
	}

	void MenuComponent::DrawGuiWindows()
	{
		if (!strcmp(Name_, "Windows"))
		{
			int index = 0;

			for (auto& window : engine->editorManager->GetAllWindowsByRef())
			{
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