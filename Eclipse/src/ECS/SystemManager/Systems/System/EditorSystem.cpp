#include "pch.h"
#include "EditorSystem.h"
#include "Editor/ECGuiAPI/ECGuiInputHandler.h"

namespace Eclipse
{
	void EditorSystem::Update()
	{
		if (engine->GetEditorState())
		{
			ECGuiInputHandler::Update();

			for (const auto& window : engine->editorManager->GetAllWindowsByRef())
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
