#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	struct sSelection
	{
		std::string name{};
		bool active{ false };
	};

	class DebugWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();

		void RunSettingsSelection(ImGuiTextFilter& filter);
		void RunSettingsDetails();
		void UpdateSelectionTrackerID(sSelection& s, int index);
		void AddInputController();
		void ShowInputList();
	private:
		ECVec2 WindowSize{ 0.0f, 0.0f };
		std::string CurrentSelection{ "Graphics" };
		std::string ToBeRemoved{};
		int PreviousIndex{ 0 };
		std::vector<sSelection> SelectionList;
		std::map<std::string, std::string> KeyMappings;
	};
}
