#include "pch.h"
#include "EditorManager.h"
#include "ImGui/Vendor/IconsFontAwesome.h"
#include "Editor/Windows/Inspector/Inspector.h"
#include "Editor/Windows/Hierarchy/Hierarchy.h"
#include "Editor/Windows/Scene/Scene.h"
#include "Editor/Windows/GameView/GameView.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
//#include "Library/Strings/Lexical.h"

namespace Eclipse
{
	EditorManager::EditorManager()
	{
		InitMenu();
		InitGUIWindows();
		InitFont();
	}

	void EditorManager::InitGUIWindows()
	{
		AddWindow<Scene>("Scene");
		AddWindow<eGameView>("GameView");
		AddWindow<Inspector>("Inspector");
		AddWindow<Hierarchy>("Hierarchy");
	}

	void EditorManager::InitMenu()
	{
		MenuComponent file{ "File", EditorMenuType::FILE };
		file.AddItems("Exit");
		MenuComponent window{ "Windows", EditorMenuType::WINDOWS };

		MenuBar_.AddMenuComponents(file);
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
		glTexImage2D(GL_TEXTURE_2D, 0, bytes_per_pixels, width, height, 0, (bytes_per_pixels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		io.Fonts->SetTexID((void*)static_cast<size_t>(textureID));

		// Initialize ImGui Icons
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("src/ImGui/Vendor/fontawesome-webfont.ttf", 12.0f, &icons_config, icons_ranges);
	}

	Entity EditorManager::CreateEntity(EntityType type)
	{
		Entity ID = engine->world.CreateEntity();
		engine->world.AddComponent(ID, EntityComponent{ type, lexical_cast<std::string>(type), true});
		engine->world.AddComponent(ID, TransformComponent{});

		EntityHierarchyList_.push_back(ID);
		EntityToTypeMap_.insert(std::pair<Entity, EntityType>(ID, type));
		GEHIndex_ = EntityHierarchyList_.size() - 1;

		return ID;
	}

	void EditorManager::DestroyEntity(Entity ID)
	{
		for (size_t i = 0; i < EntityHierarchyList_.size(); ++i)
		{
			if (ID == EntityHierarchyList_[i])
			{
				EntityHierarchyList_.erase(EntityHierarchyList_.begin() + i);

				if (i == EntityHierarchyList_.size())
					GEHIndex_ = EntityHierarchyList_.size() - 1;
				else
					GEHIndex_ = i;

				break;
			}
		}

		EntityToTypeMap_.erase(ID);
		engine->world.DestroyEntity(ID);
	}

	std::vector<std::unique_ptr<ECGuiWindow>>& EditorManager::GetAllWindows()
	{
		return Windows_;
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
		return EntityHierarchyList_[GEHIndex_];
	}
}
