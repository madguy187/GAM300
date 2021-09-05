#include "pch.h"
#include "ECGuiInputHandler.h"

namespace Eclipse
{
	void ECGuiInputHandler::Update()
	{
		ImGuiIO& io = ImGui::GetIO();

		// REDO/UNDO
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)) && io.KeyCtrl)
			CommandHistory::Undo();
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Y)) && io.KeyCtrl)
			CommandHistory::Redo();
		// Delete Entity
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
		{
			if (!engine->editorManager->IsEntityListEmpty())
			{
				Entity currEnt = engine->editorManager->GetSelectedEntity();

				if (currEnt != engine->gCamera.GetEditorCameraID() ||
					currEnt != engine->gCamera.GetGameCameraID())
				{
					engine->editorManager->DestroyEntity(currEnt);
				}
			}
		}
	}
}
