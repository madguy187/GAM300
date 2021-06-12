#include "pch.h"
#include "Hierarchy.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	void Hierarchy::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&Hierarchy::DrawImpl, this));
	}

	Hierarchy::Hierarchy()
	{
		Type = EditorWindowType::HIERARCHY;
		WindowName = "Hierarchy";
	}

	void Hierarchy::DrawImpl()
	{
		ECGui::DrawTextWidget<size_t>("Entity Count", engine->editorManager->EntityHierarchyList_.size());
		ECGui::InsertHorizontalLineSeperator();

		static ImGuiTextFilter EntFilter;
		EntFilter.Draw();
		ECGui::InsertHorizontalLineSeperator();

		TrackEntitySelection(engine->editorManager->EntityHierarchyList_, CurrEnt_, 
			PrevEnt_, engine->editorManager->GEHIndex_, EntFilter);
	}

	void Hierarchy::TrackEntitySelection(const std::vector<Entity>& list, EntitySelectionTracker& curr, 
		EntitySelectionTracker& prev, size_t& globalIndex, ImGuiTextFilter& filter)
	{
		std::string entityName{};

		for (size_t index = 0; index < list.size(); ++index)
		{
			auto& entCom = engine->world.GetComponent<EntityComponent>(list[index]);

			if (filter.PassFilter(entCom.Name.c_str()))
			{
				if (ECGui::CreateSelectableButton(my_strcat(entCom.Name, " ", list[index]).c_str(),
					&entCom.IsActive))
				{
					if (curr.index == list[index])
					{
						entCom.IsActive = true;
						continue;
					}

					if (!curr.name.empty())
					{
						prev.name = curr.name;
						prev.index = curr.index;
					}

					curr.name = entityName;
					curr.index = list[index];

					if (!prev.name.empty() && curr.name != prev.name)
					{
						bool deleted = true;

						if (std::find(list.begin(), list.end(), prev.index) != list.end())
							deleted = false;

						if (!deleted)
						{
							auto& prevEntCom = engine->world.GetComponent<EntityComponent>(prev.index);
							prevEntCom.IsActive = false;
						}
					}

					globalIndex = index;
				}
			}
		}
	}
}
