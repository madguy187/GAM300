#include "pch.h"
#include "EditorSystem.h"

namespace Eclipse
{
	void EditorSystem::Update()
	{
		if (engine->GetEditorState())
		{
			for (const auto& window : engine->editorManager->GetAllWindows())
			{
				window->Update();
			}
			
			engine->editorManager->GetMenuBar().Update();
		}
	}

	void EditorSystem::SaveTemp()
	{
		if (engine->GetEditorState())
		{
			engine->editorManager->SaveTemp();
		}
	}

	void EditorSystem::LoadTemp()
	{
		if (engine->GetEditorState())
		{
			engine->editorManager->LoadTemp();
		}
	}
}
