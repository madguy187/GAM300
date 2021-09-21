#include "pch.h"
#include "EditorManager.h"
#include "ImGui/Vendor/IconsFontAwesome.h"
#include "ImGui/Vendor/IconsMaterialDesignIcons.h"
#include "Editor/Windows/Inspector/Inspector.h"
#include "Editor/Windows/Hierarchy/Hierarchy.h"
#include "Editor/Windows/Scene/SceneView.h"
#include "Editor/Windows/GameView/GameView.h"
#include "Editor/Windows/Debug/Debug.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "Editor/Windows/AssetBrowser/AssetBrowser.h"
#include "Editor/Windows/Log/Log.h"
#include "Editor/Windows/Profiler/Profiler.h"
#include "Editor/Windows/SwitchViews/TopSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/BottomSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/LeftSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/RightSwitchViewWindow.h"
#include "Editor/Windows/Header/HeaderWindow.h"

namespace Eclipse
{
	EditorManager::EditorManager()
	{
		InitMenu();
		InitGUIWindows();
		InitFont();

		EDITOR_LOG_INFO("Editor Initialized!");
		EDITOR_LOG_WARN("Testing Warning!");
	}

	void EditorManager::InitGUIWindows()
	{
		AddWindow<eGameViewWindow>("Game Viewport");
		AddWindow<SceneWindow>("Scene Viewport");
		AddWindow<InspectorWindow>("Inspector");
		AddWindow<HierarchyWindow>("Hierarchy");
		AddWindow<ProfilerWindow>("Profiler");
		AddWindow<AssetBrowserWindow>("Asset Browser");
		AddWindow<LoggerWindow>("Log");
		AddWindow<DebugWindow>("Debug");
		AddWindow<TopSwitchViewWindow>("Top Viewport");
		AddWindow<BottomSwitchViewWindow>("Bottom Viewport");
		AddWindow<LeftSwitchViewWindow>("Left Viewport");
		AddWindow<RightSwitchViewWindow>("Right Viewport");
		AddWindow<HeaderWindow>("Header");

		for (const auto& window : Windows_)
		{
			window->Init();
		}
	}

	void EditorManager::InitMenu()
	{
		MenuComponent file{ "File", EditorMenuType::FILE };
		file.AddItems("New");
		file.AddItems("Open");
		file.AddItems("Save As...");
		file.AddItems("Exit");
		MenuBar_.AddMenuComponents(file);

		MenuComponent edit{ "Edit", EditorMenuType::EDIT };
		edit.AddItems("Undo");
		edit.AddItems("Redo");
		MenuBar_.AddMenuComponents(edit);

		MenuComponent window{ "Windows", EditorMenuType::WINDOWS };
		MenuBar_.AddMenuComponents(window);
	}

	void EditorManager::InitFont()
	{
		// Initialize Custom ImGui Text
		// io.Fonts->AddFontDefault();
		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig text_config;
		text_config.OversampleH = 2;
		text_config.OversampleV = 2;
		text_config.GlyphExtraSpacing.x = 1.0f;
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/OpenSans-SemiBold.ttf", 14.0f, &text_config);
		unsigned char* pixels;
		int width, height, bytes_per_pixels;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixels);

		GLuint textureID;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D, 0, bytes_per_pixels, width, height, 0, 
			(bytes_per_pixels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		io.Fonts->SetTexID((void*)static_cast<size_t>(textureID));

		// Initialize ImGui Icons
		/*static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/fontawesome-webfont.ttf", 12.0f, &icons_config, icons_ranges);*/
		static const ImWchar icons_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
		static const ImWchar icons_ranges2[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/materialdesignicons-webfont.ttf", 12.0f, &icons_config, icons_ranges);
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/fontawesome-webfont.ttf", 12.0f, &icons_config, icons_ranges2);
	}

	Entity EditorManager::CreateDefaultEntity(EntityType type)
	{
		Entity ID = engine->world.CreateEntity();

		engine->world.AddComponent(ID, EntityComponent{ type, lexical_cast_toStr<EntityType>(type), true });
		engine->world.AddComponent(ID, TransformComponent{});

		// Check this please - Rachel
		if(type != EntityType::ENT_GAMECAMERA)
		{
			auto& _transform = engine->world.GetComponent<TransformComponent>(ID);
			engine->gPicker.GenerateAabb(ID, _transform, type);

		}
		
		EntityHierarchyList_.push_back(ID);
		EntityToIndexMap_.insert(std::pair<Entity, int>(ID, static_cast<int>(EntityHierarchyList_.size() - 1)));
		GEHIndex_ = EntityHierarchyList_.size() - 1;
		SetSelectedEntity(ID);

		return ID;
	}

	void EditorManager::RegisterExistingEntity(Entity ID)
	{
		EntityHierarchyList_.push_back(ID);
		EntityToIndexMap_.insert(std::pair<Entity, int>(ID, static_cast<int>(EntityHierarchyList_.size() - 1)));
		GEHIndex_ = EntityHierarchyList_.size() - 1;
		SetSelectedEntity(ID);
	}

	void EditorManager::DestroyEntity(Entity ID)
	{
		size_t pos = static_cast<size_t>(EntityToIndexMap_[ID]);

		EntityHierarchyList_.erase(EntityHierarchyList_.begin() + pos);
		EntityToIndexMap_.erase(ID);

		if (!EntityHierarchyList_.empty())
		{
			if (pos == EntityHierarchyList_.size())
				SetSelectedEntity(EntityHierarchyList_[EntityHierarchyList_.size() - 1]);
			else
				SetSelectedEntity(EntityHierarchyList_[pos]);
		}

		engine->world.DestroyEntity(ID);
	}

	void EditorManager::SwapEntities(size_t firstIndex, size_t secondIndex)
	{
		EntityToIndexMap_[EntityHierarchyList_[firstIndex]] = static_cast<int>(secondIndex);
		EntityToIndexMap_[EntityHierarchyList_[secondIndex]] = static_cast<int>(firstIndex);;

		auto tmp = EntityHierarchyList_[firstIndex];
		EntityHierarchyList_[firstIndex] = EntityHierarchyList_[secondIndex];
		EntityHierarchyList_[secondIndex] = tmp;

		SetSelectedEntity(tmp);
	}

	void EditorManager::InsertExistingEntity(size_t pos, Entity ID)
	{
		auto oldItrPos = std::find(EntityHierarchyList_.begin(), EntityHierarchyList_.end(), ID);

		// Erase if it already exist, safety check to remove duplicates
		if (oldItrPos != EntityHierarchyList_.end())
			EntityHierarchyList_.erase(oldItrPos);

		auto itrPos = EntityHierarchyList_.begin() + pos;
		EntityHierarchyList_.insert(itrPos, ID);
		// No need check for dup here, it will just replace
		EntityToIndexMap_[ID] = static_cast<int>(pos);

		for (auto& pair : EntityToIndexMap_)
		{
			if (pair.second >= pos && pair.first != ID)
				pair.second++;
		}

		SetSelectedEntity(ID);
	}

	std::vector<std::unique_ptr<ECGuiWindow>>& EditorManager::GetAllWindowsByRef()
	{
		return Windows_;
	}

	const std::vector<Entity>& EditorManager::GetEntityListByConstRef() const
	{
		return EntityHierarchyList_;
	}

	const Entity* EditorManager::GetEntityListData()
	{
		return EntityHierarchyList_.data();
	}

	MenuBar& EditorManager::GetMenuBar()
	{
		return MenuBar_;
	}

	size_t EditorManager::GetWindowListSize() const
	{
		return Size_;
	}

	Entity EditorManager::GetSelectedEntity() const
	{
		if (!EntityHierarchyList_.empty())
			return EntityHierarchyList_[GEHIndex_];
		else
			return MAX_ENTITY;
	}

	size_t EditorManager::GetEntityListSize() const
	{
		return EntityHierarchyList_.size();
	}

	int EditorManager::GetEntityIndex(Entity ID)
	{
		return EntityToIndexMap_[ID];
	}

	Entity EditorManager::GetEntityID(int index)
	{
		return EntityHierarchyList_[index];
	}

	bool EditorManager::IsEntityListEmpty() const
	{
		return EntityHierarchyList_.empty();
	}

	void EditorManager::SetSelectedEntity(Entity ID)
	{
		GEHIndex_ = static_cast<size_t>(EntityToIndexMap_[ID]);
		auto* hc = GetEditorWindow<HierarchyWindow>();
		hc->UpdateEntityTracker(ID);
	}

	void EditorManager::SetGlobalIndex(size_t index)
	{
		GEHIndex_ = index;
	}

	void EditorManager::Clear()
	{
		EntityHierarchyList_.clear();
		EntityToIndexMap_.clear();
		GEHIndex_ = 0;

		for (const auto& window : Windows_)
		{
			window->Unload();
		}
	}
}
