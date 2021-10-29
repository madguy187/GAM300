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
		unsigned int activeCounter = 0;
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();
		void TrackEntitySelection(const std::vector<Entity>& list, EntitySelectionTracker& prev,
			EntitySelectionTracker& curr, ImGuiTextFilter& filter);

		void ParentRecursion(EntityComponent& entCom, Entity Num, const std::vector<Entity>& list, EntitySelectionTracker& prev,
			EntitySelectionTracker& curr);

		void ShowEntityCreationList();
		void UpdateEntityTracker(Entity ID);
		size_t GetEntityGlobalIndex(size_t data);
		std::string GetEntityComponentEntityNumber(std::string EntityName);
		size_t ConvertEntityStringtoNumber(std::string EntityNumber);
		void HightLightParentAndChild(EntityComponent& Parent);
		void ShowCreateModelList();
		int GetListPos(size_t currIndex);
	private:
		EntitySelectionTracker CurrEnt_;
		EntitySelectionTracker PrevEnt_;
		std::vector<std::vector<std::string>> TagList_;
	};
}
