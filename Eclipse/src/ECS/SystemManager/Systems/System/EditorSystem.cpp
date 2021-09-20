#include "pch.h"
#include "EditorSystem.h"
#include "Editor/ECGuiAPI/ECGuiInputHandler.h"

namespace Eclipse
{
	void EditorSystem::Update()
	{
		engine->Timer.SetName({ SystemName::EDITOR });
		engine->Timer.tracker.system_start = glfwGetTime();

		if (engine->GetEditorState())
		{
			ECGuiInputHandler::Update();

			for (const auto& window : engine->editorManager->GetAllWindowsByRef())
			{
				window->Update();
			}
			
			engine->editorManager->GetMenuBar().Update();
		}

		engine->Timer.tracker.system_end = glfwGetTime();
		engine->Timer.ContainerAddTime(engine->Timer.tracker);
	}

}
