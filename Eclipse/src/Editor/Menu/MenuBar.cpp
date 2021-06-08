#include "pch.h"
#include "MenuBar.h"

namespace Eclipse
{
	void MenuBar::Update()
	{
		ECGui::DrawMenuBar<void()>(std::bind(&MenuBar::DrawImpl, this));
	}

	void MenuBar::DrawImpl()
	{
		for (auto& member : MenuComponents_)
		{
			ECGui::DrawMenuComponent<void()>(member.GetName(),
											 [&]() { member.Update(); });
		}
	}

	void MenuBar::AddMenuComponents(MenuComponent com)
	{
		MenuComponents_.push_back(com);
	}

	MenuComponent* MenuBar::GetMenuComponent(EditorMenuType type)
	{
		for (auto& member : MenuComponents_)
		{
			if (member.GetType() == type)
				return &member;
		}

		return nullptr;
	}
}
