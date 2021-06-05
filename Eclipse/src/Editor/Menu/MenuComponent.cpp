#include "pch.h"
#include "MenuComponent.h"

namespace Eclipse
{
	void MenuComponent::Update()
	{
		ECGui::DrawMenuBar<void()>(std::bind(&MenuComponent::DrawImpl, this));
	}

	MenuComponent::MenuComponent(const char* name, EditorMenuType type) :
		Name_{ name }, Type_{ type } {}

	void MenuComponent::AddItems(const char* name)
	{
		List_.push_back(std::string{ name });
		ListToIndex_[name] = ID;
		ID++;
	}

	void MenuComponent::DrawImpl()
	{
		for (const auto& str : List_)
		{
			switch (ListToIndex_[str.c_str()])
			{
			case 0:
			{

			}
			default:
				break;
			}
		}
	}

	EditorMenuType MenuComponent::GetType()
	{
		return Type_;
	}
}