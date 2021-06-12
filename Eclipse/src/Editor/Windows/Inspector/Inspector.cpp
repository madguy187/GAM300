#include "pch.h"
#include "Inspector.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	void Inspector::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&Inspector::DrawImpl, this));
	}

	Inspector::Inspector()
	{
		Type = EditorWindowType::INSPECTOR;
		WindowName = "Inspector";
	}

	void Inspector::DrawImpl()
	{
		WindowSize_.setX(ECGui::GetWindowSize().x);
		WindowSize_.setY(ECGui::GetWindowSize().y);

		if (!engine->editorManager->EntityHierarchyList_.empty())
		{
			Entity currEnt = engine->editorManager->EntityHierarchyList_[engine->editorManager->GEHIndex_];
			auto& entcom = engine->world.GetComponent<EntityComponent>(currEnt);

			ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entcom.Name.c_str()),
				entcom.Name.size());

			ECGui::InsertHorizontalLineSeperator();

			ShowEntityProperty(currEnt);
			ShowTransformProperty(currEnt);
		}
		else
		{
			const char* entName = "No Entities";

			ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entName),
				strlen(entName));
		}

		ECGui::InsertHorizontalLineSeperator();
	}

	bool Inspector::ShowEntityProperty(Entity ID)
	{
		//if (engine->world.CheckComponent(ID, ))
		return false;
	}

	bool Inspector::ShowTransformProperty(Entity ID)
	{
		return false;
	}
}