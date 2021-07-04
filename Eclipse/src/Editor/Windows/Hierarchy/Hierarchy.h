#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	struct EntitySelectionTracker
	{
		Entity index{ 0 };
		std::string name{};
	};

	class HierarchyWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		HierarchyWindow();
		void DrawImpl();
		void TrackEntitySelection(const std::vector<Entity>& list, EntitySelectionTracker& prev,
			EntitySelectionTracker& curr, size_t& globalIndex, ImGuiTextFilter& filter);
		void ShowEntityCreationList();
		void UpdateEntityTracker(Entity ID);
	private:
		EntitySelectionTracker CurrEnt_;
		EntitySelectionTracker PrevEnt_;
		std::vector<std::vector<std::string>> TagList_;
	};
}
