#pragma once

namespace Eclipse
{
	class MenuComponent
	{
	public:
		void Update();

		MenuComponent(const char* name, EditorMenuType type);
		void AddItems(const char* name);
		void DrawImpl(const char* key);
		EditorMenuType GetType();
		const char* GetName();
	private:
		const char* Name_;
		EditorMenuType Type_;
		std::vector<std::string> List_;
		std::unordered_map<const char*, const char*> ListToName_;
	};
}