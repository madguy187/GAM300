#include "pch.h"
#include "MenuComponent.h"
#include "Editor/Windows/Scene/Scene.h"
#include "Editor/Windows/GameView/GameView.h"

namespace Eclipse
{
	void MenuComponent::Update()
	{
		for (const auto& str : List_)
		{
			DrawImpl(str.c_str());
		}
	}

	MenuComponent::MenuComponent(const char* name, EditorMenuType type) :
		Name_{ name }, Type_{ type } {}

	void MenuComponent::AddItems(const char* name)
	{
		List_.push_back(std::string{ name });
		ListToName_[name] = name;
	}

	void MenuComponent::DrawImpl(const char* key)
	{
		if (!strcmp(key, "Scene"))
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