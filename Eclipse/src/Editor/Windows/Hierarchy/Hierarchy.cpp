#include "pch.h"
#include "Hierarchy.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	void HierarchyWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&HierarchyWindow::DrawImpl, this));
	}

	void HierarchyWindow::Unload()
	{
		CurrEnt_.Clear();
		PrevEnt_.Clear();
	}

	HierarchyWindow::HierarchyWindow()
	{
		Type = EditorWindowType::EWT_HIERARCHY;
		WindowName = "Hierarchy";

		// For Geometry
		TagList_.push_back(std::vector<std::string>());
		// For Lights
		TagList_.push_back(std::vector<std::string>());

		for (int index = 0; index != static_cast<int>(EntityType::ENT_LIGHT_POINT); ++index)
		{
			EntityType temp = static_cast<EntityType>(index);
			TagList_[0].push_back(lexical_cast_toStr<EntityType>(temp));
		}

		for (int index = static_cast<int>(EntityType::ENT_LIGHT_POINT); 
			index != static_cast<int>(EntityType::ENT_UNASSIGNED); ++index)
		{
			EntityType temp = static_cast<EntityType>(index);
			TagList_[1].push_back(lexical_cast_toStr<EntityType>(temp));
		}
	}

	void HierarchyWindow::DrawImpl()
	{
		PopUpButtonSettings settings{ "Add Entity", "EntityCreationListBegin" };
		ECGui::BeginPopUpButtonList<void()>(settings, std::bind(&HierarchyWindow::ShowEntityCreationList, this));
		ECGui::InsertHorizontalLineSeperator();

		ECGui::DrawTextWidget<size_t>("Entity Count", engine->editorManager->GetEntityListSize());
		ECGui::InsertHorizontalLineSeperator();

		static ImGuiTextFilter EntFilter;
		EntFilter.Draw();
		ECGui::InsertHorizontalLineSeperator();

		if (ECGui::BeginTreeNode("Default Scene"))
		{
			TrackEntitySelection(engine->editorManager->GetEntityListByConstRef(), 
				PrevEnt_, CurrEnt_, EntFilter);

			ECGui::EndTreeNode();
		}
	}

	void HierarchyWindow::TrackEntitySelection(const std::vector<Entity>& list, EntitySelectionTracker& prev,
		EntitySelectionTracker& curr, ImGuiTextFilter& filter)
	{
		std::string entityName{};

		for (size_t index = 0; index < list.size(); ++index)
		{
			auto& entCom = engine->world.GetComponent<EntityComponent>(list[index]);

			if (filter.PassFilter(entCom.Name.c_str()))
			{
				entityName = my_strcat(entCom.Name, " ", list[index]);
				if (ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
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

					engine->editorManager->SetGlobalIndex(index);
				}
			}
		}
	}

	void HierarchyWindow::ShowEntityCreationList()
	{
		for (size_t i = 0; i < TagList_.size(); ++i)
		{
			switch (i)
			{
				case 0:
				{
					if (ECGui::BeginTreeNode("Geometry"))
					{
						for (size_t j = 0; j < TagList_[i].size(); ++j)
						{
							bool selected = false;

							if (ECGui::CreateSelectableButton(TagList_[i][j].c_str(), &selected))
							{
								Entity ID = engine->editorManager->CreateDefaultEntity(lexical_cast_toEnum<EntityType>(TagList_[i][j]));
								engine->GraphicsManager.CreatePrimitives(ID, static_cast<int>(i * TagList_.size() + j));
								UpdateEntityTracker(ID);
							}
						}

						ECGui::EndTreeNode();
					}

					break;
				}
				case 1:
				{
					if (ECGui::BeginTreeNode("Lights"))
					{
						for (size_t j = 0; j < TagList_[i].size(); ++j)
						{
							bool selected = false;

							if (ECGui::CreateSelectableButton(TagList_[i][j].c_str(), &selected))
							{
								Entity ID = engine->editorManager->CreateDefaultEntity(lexical_cast_toEnum<EntityType>(TagList_[i][j]));
								engine->GraphicsManager.CreatePrimitives(ID, static_cast<int>(i * TagList_[i - 1].size() + j));
								UpdateEntityTracker(ID);
							}
						}

						ECGui::EndTreeNode();
					}

					break;
				}
				default:
					break;
			}
		}
	}
	void HierarchyWindow::UpdateEntityTracker(Entity ID)
	{
		if (ID != CurrEnt_.index)
		{
			auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

			if (engine->world.CheckComponent<EntityComponent>(CurrEnt_.index))
			{
				auto& prevEntCom = engine->world.GetComponent<EntityComponent>(CurrEnt_.index);
				prevEntCom.IsActive = false;
				PrevEnt_.name = CurrEnt_.name;
				PrevEnt_.index = CurrEnt_.index;
			}

			CurrEnt_.name = my_strcat(entCom.Name, " ", ID);
			CurrEnt_.index = ID;
			entCom.IsActive = true;
		}
	}
}
