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
#include "Editor/Windows/MeshEditor/MeshEditor.h"

namespace Eclipse
{
	EditorManager::EditorManager()
	{
		InitMenu();
		InitGUIWindows();
		InitFont();

		EDITOR_LOG_INFO("Editor Initialized!");
	}

	void EditorManager::InitGUIWindows()
	{
		AddWindow<eGameViewWindow>("Game Viewport " ICON_MDI_MONITOR);
		AddWindow<SceneWindow>("Scene Viewport " ICON_MDI_MONITOR);
		AddWindow<TopSwitchViewWindow>("Top Viewport " ICON_MDI_MONITOR);
		AddWindow<BottomSwitchViewWindow>("Bottom Viewport " ICON_MDI_MONITOR);
		AddWindow<LeftSwitchViewWindow>("Left Viewport " ICON_MDI_MONITOR);
		AddWindow<RightSwitchViewWindow>("Right Viewport " ICON_MDI_MONITOR);

		AddWindow<InspectorWindow>("Inspector " ICON_MDI_MAGNIFY_SCAN);
		AddWindow<HierarchyWindow>("Hierarchy " ICON_MDI_FILE_TREE);
		AddWindow<ProfilerWindow>("Profiler " ICON_MDI_FILE_PERCENT);
		AddWindow<AssetBrowserWindow>("Asset Browser " ICON_MDI_FILE_IMAGE);
		AddWindow<LoggerWindow>("Log " ICON_MDI_POST);
		AddWindow<DebugWindow>("Settings " ICON_MDI_ACCOUNT_COG);
		AddWindow<HeaderWindow>("Header " ICON_MDI_PAGE_LAYOUT_HEADER);
		AddWindow<MeshEditorWindow>("Mesh Editor");

		for (const auto& window : Windows_)
		{
			window->Init();
		}
	}

	void EditorManager::InitMenu()
	{
		MenuComponent file{ "File" ICON_MDI_FILE, EditorMenuType::FILE };
		file.AddItems("New " ICON_MDI_FOLDER_PLUS);
		file.AddItems("Open " ICON_MDI_FOLDER_OPEN);
		file.AddItems("Save " ICON_MDI_CONTENT_SAVE);
		file.AddItems("Save As... " ICON_MDI_CONTENT_SAVE_EDIT);
		file.AddItems("Exit " ICON_MDI_EXIT_TO_APP);
		MenuBar_.AddMenuComponents(file);

		MenuComponent edit{ "Edit" ICON_MDI_PENCIL, EditorMenuType::EDIT };
		edit.AddItems("Undo " ICON_MDI_UNDO_VARIANT);
		edit.AddItems("Redo " ICON_MDI_REDO_VARIANT);
		MenuBar_.AddMenuComponents(edit);

		MenuComponent window{ "Windows" ICON_MDI_MONITOR, EditorMenuType::WINDOWS };
		MenuBar_.AddMenuComponents(window);

		MenuComponent style{ "Style" ICON_MDI_ACCOUNT, EditorMenuType::STYLE };
		style.AddItems("Oppa GuanHin Style");
		style.AddItems("Oppa Nico Style");
		style.AddItems("Oppa Fikrul Style");
		style.AddItems("Oppa TianYu Style");
		style.AddItems("Oppa Janelle Style");
		style.AddItems("Oppa Denny Style");
		style.AddItems("Oppa Brina Style");
		style.AddItems("Oppa Rachel Style");
		style.AddItems("Oppa Darren Style");
		style.AddItems("Oppa JianHerng Style");
		MenuBar_.AddMenuComponents(style);

		MenuComponent aboutus{ "About Eclipse" ICON_MDI_CROSSHAIRS_QUESTION, EditorMenuType::ABOUTUS };
		MenuBar_.AddMenuComponents(aboutus);
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
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/OpenSans-SemiBold.ttf", 16.0f, &text_config);
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
		//static const ImWchar icons_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/fontawesome-webfont.ttf", 14.0f, &icons_config, icons_ranges);
		//"src/ImGui/Vendor/fontawesome-webfont.ttf"
		static const ImWchar icons_ranges2[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
		ImFontConfig icons_config2; icons_config2.MergeMode = true; icons_config2.PixelSnapH = true;  
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/materialdesignicons-webfont.ttf", 16.0f, &icons_config2, icons_ranges2);
	}

	Entity EditorManager::CreateDefaultEntity(EntityType type)
	{
		Entity ID = engine->world.CreateEntity();

		engine->world.AddComponent(ID, EntityComponent{ type, lexical_cast_toStr<EntityType>(type), true });
		engine->world.AddComponent(ID, TransformComponent{});

		// Check this please - Rachel
		if (type != EntityType::ENT_GAMECAMERA)
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

		if (engine->world.CheckComponent<MaterialComponent>(ID))
			engine->MaterialManager.HighlightClick(ID);
	}

	void EditorManager::DestroyEntity(Entity ID)
	{
		size_t pos = static_cast<size_t>(EntityToIndexMap_[ID]);

		EntityHierarchyList_.erase(EntityHierarchyList_.begin() + pos);
		EntityToIndexMap_.erase(ID);

		if (!EntityHierarchyList_.empty())
		{
			if (pos >= EntityHierarchyList_.size())
			{
				SetSelectedEntity(EntityHierarchyList_[EntityHierarchyList_.size() - 1]);
			}
			else
			{
				SetSelectedEntity(EntityHierarchyList_[pos - 1]);

				for (auto& pair : EntityToIndexMap_)
				{
					if (pair.second > pos - 1)
						pair.second--;
				}
			}
		}

		if (engine->world.CheckComponent<MaterialComponent>(GetSelectedEntity()))
			engine->MaterialManager.HighlightClick(GetSelectedEntity());

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

	bool EditorManager::IsAnyGizmoWindowActive()
	{
		auto* scene = dynamic_cast<SceneWindow*>(Windows_[1].get());
		auto* sv1 = dynamic_cast<TopSwitchViewWindow*>(Windows_[2].get());
		auto* sv2 = dynamic_cast<BottomSwitchViewWindow*>(Windows_[3].get());
		auto* sv3 = dynamic_cast<LeftSwitchViewWindow*>(Windows_[4].get());
		auto* sv4 = dynamic_cast<RightSwitchViewWindow*>(Windows_[5].get());

		if (scene->GetIsWindowActive() || sv1->GetIsWindowActive() || sv2->GetIsWindowActive()
			|| sv3->GetIsWindowActive() || sv4->GetIsWindowActive())
		{
			return true;
		}
		else
		{
			return false;
		}
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
