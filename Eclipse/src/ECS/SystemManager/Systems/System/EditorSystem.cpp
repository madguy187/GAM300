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
}
