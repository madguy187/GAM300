#include "pch.h"
#include "Editor/Utilities/OpenFileDialog/OpenFileDialog.h"
#include "Editor/Windows/GameView/GameView.h"
#include "ECGuiInputHandler.h"

namespace Eclipse
{
	void ECGuiInputHandler::Update()
	{
		if (engine->GetEditorState())
		{
			ImGuiIO& io = ImGui::GetIO();

			// REDO/UNDO
			if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_Z)) && io.KeyCtrl)
				CommandHistory::Undo();
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_Y)) && io.KeyCtrl)
				CommandHistory::Redo();
			// Delete Entity
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_Delete)))
			{
				if (!engine->editorManager->IsEntityListEmpty())
				{
					Entity currEnt = engine->editorManager->GetSelectedEntity();

					if (currEnt != engine->gCamera.GetEditorCameraID() &&
						currEnt != engine->gCamera.GetGameCameraID())
					{
						//Remove function for DynamicAABBTree. -Rachel
						engine->gDynamicAABBTree.RemoveData(currEnt);
						engine->gCullingManager->Remove(currEnt);

						engine->editorManager->DestroyEntity(currEnt);

						engine->gPhysics.RemoveActor(currEnt);

						//Please check this! Resets the selected object's ID to the editor's selected entity. - Rachel
						engine->gPicker.SetCurrentCollisionID(engine->editorManager->GetSelectedEntity());
					}
				}
			}
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_O)) && io.KeyCtrl)
			{
				FileDialog::FileBrowser();
			}
			// ImGuizmo Change
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_Q)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ECGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(-1);
			}
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_W)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ECGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
			}
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_E)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ECGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(ImGuizmo::OPERATION::SCALE);
			}
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_R)))
			{
				if (engine->editorManager->IsAnyGizmoWindowActive())
					if (!ImGuizmo::IsUsing() && !ECGui::IsMouseDown(1))
						engine->editorManager->GetEditorWindow<SceneWindow>()->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
			}
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_F11)))
			{
				if (engine->GetPlayState())
				{
					auto* gv = engine->editorManager->GetEditorWindow<eGameViewWindow>();

					if (!gv->GetIsViewFullscreen())
					{
						gv->SetViewToFullscreen(true);
						gv->SetViewToOriginalState(false);
					}
					else
					{
						gv->SetViewToFullscreen(false);
						gv->SetViewToOriginalState(true);
					}
				}
			}
			// File Saving
			else if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_S)) && io.KeyCtrl)
				FileDialog::SaveFile();
		}
	}
}
