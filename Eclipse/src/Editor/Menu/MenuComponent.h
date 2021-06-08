#pragma once

namespace Eclipse
{
	class MenuComponent
	{
	public:
		void Update();

		MenuComponent(const char* name, EditorMenuType type);
		void AddItems(const char* name);
		void DrawImpl();
		void DrawGuiWindows();
		EditorMenuType GetType();
		const char* GetName();
	private:
		const char* Name_;
		EditorMenuType Type_;
		int ID;
		std::vector<std::string> List_;
		std::map<int, const char*> ListToName_;
	};
}