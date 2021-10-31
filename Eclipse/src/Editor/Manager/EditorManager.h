
#pragma once

#include "Editor/Windows/Interface/ECGuiWindow.h"
#include "Editor/Menu/MenuBar.h"
#include <Editor/Utilities/DragAndDrop/DragAndDrop.h>

#include "Serialization/SerializationManager.h"

namespace Eclipse
{
	class EditorManager
	{
	public:
		EditorManager();

		// Intialization
		void InitGUIWindows();
		void InitMenu();
		void InitFont();

		// Registry
		Entity CreateDefaultEntity(EntityType type);
		void RegisterExistingEntity(Entity ID);
		void InsertExistingEntity(size_t pos, Entity ID);
		void DestroyEntity(Entity ID);

		// Editor Utilities
		void SwapEntities(size_t firstIndex, size_t secondIndex);

		// Getters
		std::vector<std::unique_ptr<ECGuiWindow>>& GetAllWindowsByRef();
		const std::vector<Entity>& GetEntityListByConstRef() const;
		const Entity* GetEntityListData();
		MenuBar& GetMenuBar();
		size_t GetWindowListSize() const;
		Entity GetSelectedEntity() const;
		size_t GetEntityListSize() const;
		int GetEntityIndex(Entity ID);
		Entity GetEntityID(int index);
		bool IsEntityListEmpty() const;
		bool IsAnyGizmoWindowActive();
		bool IsAnySwitchWindowHovered();
		bool GetMeshEditorActive() const;
		bool GetRecoveryFileExistence() const;

		// Setters
		void SetSelectedEntity(Entity ID);
		void SetGlobalIndex(size_t index);
		void SetMeshEditorActive(bool active);
		void SetRecoveryFileExistence(bool exist);

		template <typename TWindow>
		TWindow* GetEditorWindow();
		// Cleaning Up
		void Clear();

		int spriteIcon_;
		int FolderIcon_;
		int nodeHeader_;
		void TextureIconInit();

		DragAndDrop DragAndDropInst_;
	private:
		std::vector<std::unique_ptr<ECGuiWindow>> Windows_;
		MenuBar MenuBar_;
		size_t Size_{ 0 };

		std::vector<Entity> EntityHierarchyList_;
		std::unordered_map<Entity, int> EntityToIndexMap_;
		size_t GEHIndex_{ 0 };

		bool IsMeshEditorActive{ false };
		bool DoesRecoveryFileExist{ false };

		template <typename TWindow>
		inline void AddWindow(const char* title);
	};

	template <typename TWindow>
	TWindow* EditorManager::GetEditorWindow()
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

	template <typename TWindow>
	void EditorManager::AddWindow(const char* title)
	{
		static_assert(std::is_base_of_v<ECGuiWindow, TWindow>, "Type is not an ECGui Window!");
		Windows_.emplace_back(std::make_unique<TWindow>());

		auto* com = MenuBar_.GetMenuComponent(EditorMenuType::WINDOWS);
		com->AddItems(title);
		Size_++;
	}
}
