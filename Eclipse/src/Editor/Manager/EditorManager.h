#pragma once

#include "Editor/Windows/Interface/ECGuiWindow.h"
#include "Editor/Menu/MenuBar.h"
#include <Editor/Utilities/DragAndDrop.h>

namespace Eclipse
{
	class EditorManager
	{
	public:
		EditorManager();

		void InitGUIWindows();
		void InitMenu();
		void InitFont();

		Entity CreateDefaultEntity(EntityType type);
		void RegisterExistingEntity(Entity ID);
		void DestroyEntity(Entity ID);

		std::vector<std::unique_ptr<ECGuiWindow>>& GetAllWindowsByRef();
		const std::vector<Entity>& GetEntityListByConstRef() const;
		const Entity* GetEntityListData();
		MenuBar& GetMenuBar();
		size_t GetWindowListSize() const;
		Entity GetSelectedEntity() const;
		size_t GetEntityListSize() const;
		bool IsEntityListEmpty() const;

		void SetSelectedEntity(Entity ID);
		void SetGlobalIndex(size_t index);

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
		
		void Clear();

		// std::map<const char*, const char*> DataComponentFilter_;
		DragAndDrop DragAndDropInst_;
	private:
		std::vector<std::unique_ptr<ECGuiWindow>> Windows_;
		MenuBar MenuBar_;
		size_t Size_{ 0 };

		std::vector<Entity> EntityHierarchyList_;
		std::map<Entity, int> EntityToIndexMap_;
		size_t GEHIndex_{ 0 };

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
