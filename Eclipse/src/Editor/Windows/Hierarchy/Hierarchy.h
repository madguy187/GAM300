#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	struct EntitySelectionTracker
	{
		Entity index{ 0 };
		std::string name{};

		void Clear()
		{
			index = 0;
			name.clear();
		}
	};

	class HierarchyWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();
		void TrackEntitySelection(const std::vector<Entity>& list, EntitySelectionTracker& prev,
			EntitySelectionTracker& curr, ImGuiTextFilter& filter);
		void ShowEntityCreationList();
		void UpdateEntityTracker(Entity ID);
		size_t GetEntityGlobalIndex(size_t data);
		std::string GetEntityComponentEntityNumber(std::string EntityName);
		size_t ConvertEntityStringtoNumber(std::string EntityNumber);
		void TreeNodeRecursion(std::string parent, EntityComponent& entCom,EntitySelectionTracker& prev,EntitySelectionTracker& curr, size_t index);
	private:
		ImGuiTreeNodeFlags temp;
		EntitySelectionTracker CurrEnt_;
		EntitySelectionTracker PrevEnt_;
		std::vector<std::vector<std::string>> TagList_;
	};
}
