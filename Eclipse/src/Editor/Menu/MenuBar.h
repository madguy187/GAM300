#pragma once

#include "MenuComponent.h"

namespace Eclipse
{
	class MenuBar
	{
	public:
		void Update();
		void DrawImpl();
		void AddMenuComponents(MenuComponent com);
		MenuComponent* GetMenuComponent(EditorMenuType type);
		void ShowExitDialogBox(bool active);
		void ShowTeamInformation();
		static void ShowRecoveryDialogBox(bool active);
		static std::string RetrieveFilename(std::string path);

		bool GetExitStatus();
		void SetExitStatus(bool check);
	private:
		bool IsExiting{ false };
		std::vector<MenuComponent> MenuComponents_;
	};
}
