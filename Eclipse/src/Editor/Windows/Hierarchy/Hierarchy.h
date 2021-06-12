#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	struct EntitySelectionTracker
	{
		Entity index{ 0 };
		std::string name{};
	};

	class Hierarchy final : public ECGuiWindow
	{
	public:
		void Update() override;
		Hierarchy();
		void DrawImpl();
		void TrackEntitySelection(const std::vector<Entity>& list, EntitySelectionTracker& curr,
			EntitySelectionTracker& prev, size_t& globalIndex, ImGuiTextFilter& filter);
		void ShowEntityCreationList();
	private:
		EntitySelectionTracker CurrEnt_;
		EntitySelectionTracker PrevEnt_;
		std::vector<std::vector<std::string>> TagList_;
		//std::vector<std::string> LightList_;
	};
}
