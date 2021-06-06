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
		std::vector<std::unique_ptr<ECGuiWindow>>& GetAllWindows();
		MenuBar& GetMenuBar();

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

	private:
		std::vector<std::unique_ptr<ECGuiWindow>> Windows_;
		MenuBar MenuBar_;

		template <typename TWindow>
		inline void AddWindow(const char* title)
		{
			static_assert(std::is_base_of_v<ECGuiWindow, TWindow>, "Type is not an ECGui Window!");
			Windows_.emplace_back(std::make_unique<TWindow>());

			auto* com = MenuBar_.GetMenuComponent(EditorMenuType::WINDOWS);
			com->AddItems(title);
		}
	};
}
