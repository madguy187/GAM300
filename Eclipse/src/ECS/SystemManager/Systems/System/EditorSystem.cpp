#include "pch.h"
#include "EditorSystem.h"
#include "Editor/ECGuiAPI/ECGuiInputHandler.h"

namespace Eclipse
{
	void EditorSystem::Update()
	{
		if (engine->GetEditorState())
		{
			for (const auto& window : engine->editorManager->GetAllWindowsByRef())
			{
				window->Update();
			}
			
			engine->editorManager->GetMenuBar().Update();
		}
	}

}
