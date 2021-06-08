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
	private:
		std::vector<MenuComponent> MenuComponents_;
	};
}
