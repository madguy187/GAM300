#include "pch.h"
#include "EditorSystem.h"
#include "Editor/ECGuiAPI/ECGuiInputHandler.h"

namespace Eclipse
{
	void EditorSystem::Update()
	{
		ZoneScopedN("EDITOR SYSTEM")
		engine->Timer.SetName({ SystemName::EDITOR });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

		if (engine->GetEditorState())
		{
			MenuBar::ShowRecoveryDialogBox(engine->editorManager->GetRecoveryFileExistence());

			for (const auto& window : engine->editorManager->GetAllWindowsByRef())
			{
				window->Update();
			}
			
			engine->editorManager->GetMenuBar().Update();
		}

		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}

}
