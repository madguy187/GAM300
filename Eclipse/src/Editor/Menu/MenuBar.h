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

		bool GetExitStatus();
		void SetExitStatus(bool check);
	private:
		bool IsExiting{ false };
		std::vector<MenuComponent> MenuComponents_;
	};
}
