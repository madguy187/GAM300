#pragma once

#include "Editor/Windows/Interface/ECGuiWindow.h"
#include "Editor/Menu/MenuBar.h"

namespace Eclipse
{
	class EditorManager
	{
	public:
		EditorManager();

		void InitGUIWindows();
		void InitMenu();
		void InitFont();

		Entity CreateEntity(EntityType type);
		void DestroyEntity(Entity ID);

		std::vector<std::unique_ptr<ECGuiWindow>>& GetAllWindows();
		MenuBar& GetMenuBar();
		size_t GetWindowListSize();
		Entity GetSelectedEntity() const;

		template <typename TWindow>
		TWindow* GetEditorWindow()
		{
			TWindow* temp = nullptr;

			for (const auto& window : Windows_)
			{
				temp = dynamic_cast<TWindow*>(window.get());

				if (temp)
					break;
			}

			return temp;
		}

		std::vector<Entity> EntityHierarchyList_;
		std::map<Entity, EntityType> EntityToTypeMap_;
		std::map<const char*, const char*> DataComponentFilter_;
		size_t GEHIndex_{ 0 };
	private:
		std::vector<std::unique_ptr<ECGuiWindow>> Windows_;
		MenuBar MenuBar_;
		size_t Size_{ 0 };

		template <typename TWindow>
		inline void AddWindow(const char* title)
		{
			static_assert(std::is_base_of_v<ECGuiWindow, TWindow>, "Type is not an ECGui Window!");
			Windows_.emplace_back(std::make_unique<TWindow>());

			auto* com = MenuBar_.GetMenuComponent(EditorMenuType::WINDOWS);
			com->AddItems(title);
			Size_++;
		}
	};
}
