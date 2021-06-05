#pragma once

#include "Editor/Windows/Interface/ECGuiWindow.h"

namespace Eclipse
{
	class EditorManager
	{
	public:
		EditorManager();
		void InitGUIWindows();
		void InitMenu();
		void InitFont();

		template <typename TWindow>
		TWindow* getEditorWindow()
		{
			TWindow* temp = nullptr;

			for (const auto& window : _windows)
			{
				temp = dynamic_cast<TWindow*>(window.get());

				if (temp)
					break;
			}

			return temp;
		}

	private:
		std::vector<std::unique_ptr<ECGuiWindow>> Windows_;

		template <typename TWindow>
		inline void AddWindow()
		{
			ENGINE_LOG_ASSERT(std::is_base_of_v<ECGuiWindow, TWindow>, "Type is not an ECGui Window!");
			_windows.emplace_back(std::make_unique<TWindow>());
		}
	};
}
