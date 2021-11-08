#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	static const char* TagsAndLayersHeaders[] =
	{
		"Layers"
	};

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
		void DefaultLayerInit();

		void OnGraphicsUpdate();
		void OnInputUpdate();
		void OnTagsAndLayersUpdate();

		const std::unordered_map<int, std::string>& GetLayerList();
		size_t GetLayerListSize() const;
		const std::string& GetStringLayer(int index);
		int GetIndexLayer(const std::string& str);
		void SetLayerListSize(size_t size);

		std::map<std::string, std::string> KeyMappings;
	private:
		ECVec2 WindowSize{ 0.0f, 0.0f };
		std::string CurrentSelection{ "Graphics" };
		std::string ToBeRemoved{};
		int PreviousIndex{ 0 };
		std::vector<sSelection> SelectionList;
		std::unordered_map<int, std::string> LayerList;
		size_t LayerListSize{ 0 };
	};
}
