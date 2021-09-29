#include "pch.h"
#include "Editor/Utilities/OpenFileDialog/OpenFileDialog.h"
#include "ECGuiInputHandler.h"

namespace Eclipse
{
	void ECGuiInputHandler::Update()
	{
		if (engine->GetEditorState())
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
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_O)) && io.KeyCtrl)
			{
				FileDialog::FileBrowser();
			}
			// ImGuizmo Change
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Q)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(-1);
			}
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
			}
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_E)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(ImGuizmo::OPERATION::SCALE);
			}
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
			}
			// File Saving
			else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S)) && io.KeyCtrl)
				FileDialog::SaveAsFile();
		}
	}
}
