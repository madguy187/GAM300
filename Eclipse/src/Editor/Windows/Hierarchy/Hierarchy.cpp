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

		// For Geometry
		TagList_.push_back(std::vector<std::string>());
		// For Lights
		TagList_.push_back(std::vector<std::string>());

		for (int index = 0; index != static_cast<int>(EntityType::ENT_LIGHT_POINT); ++index)
		{
			EntityType temp = static_cast<EntityType>(index);
			TagList_[0].push_back(lexical_cast<std::string>(temp));
		}

		for (int index = static_cast<int>(EntityType::ENT_LIGHT_POINT); 
			index != static_cast<int>(EntityType::ENT_UNASSIGNED); ++index)
		{
			EntityType temp = static_cast<EntityType>(index);
			TagList_[1].push_back(lexical_cast<std::string>(temp));
		}
	}

	void Hierarchy::DrawImpl()
	{
		PopUpButtonSettings settings{ "Add Entity", "EntityCreationListBegin" };
		ECGui::BeginPopUpButtonList<void()>(settings, std::bind(&Hierarchy::ShowEntityCreationList, this));
		ECGui::InsertHorizontalLineSeperator();

		ECGui::DrawTextWidget<size_t>("Entity Count", engine->editorManager->EntityHierarchyList_.size());
		ECGui::InsertHorizontalLineSeperator();

		static ImGuiTextFilter EntFilter;
		EntFilter.Draw();
		ECGui::InsertHorizontalLineSeperator();

		if (ECGui::BeginTreeNode("Default Scene"))
		{
			TrackEntitySelection(engine->editorManager->EntityHierarchyList_, CurrEnt_,
				PrevEnt_, engine->editorManager->GEHIndex_, EntFilter);

			ECGui::EndTreeNode();
		}
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

	void Hierarchy::ShowEntityCreationList()
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
								Entity ID = engine->editorManager->CreateEntity(lexical_cast<EntityType>(TagList_[i][j]));
								engine->gGraphics.CreatePrimitives(ID, static_cast<int>(i * TagList_.size() + j));
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
								Entity ID = engine->editorManager->CreateEntity(lexical_cast<EntityType>(TagList_[i][j]));
								engine->gGraphics.CreatePrimitives(ID, static_cast<int>(i * TagList_[i - 1].size() + j));
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
}
