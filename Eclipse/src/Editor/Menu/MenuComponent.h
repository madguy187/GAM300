#pragma once

#include "Editor/ECGuiAPI/ECGui.h"

namespace Eclipse
{
	class MenuComponent
	{
	public:
		void Update();

		MenuComponent(const char* name, EditorMenuType type);
		void AddItems(const char* name);
		void DrawImpl();
		EditorMenuType GetType();
	private:
		const char* Name_;
		EditorMenuType Type_;
		int ID{ 0 };
		std::vector<std::string> List_;
		std::unordered_map<const char*, int> ListToIndex_;
	};
}