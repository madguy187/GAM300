#include "pch.h"
#include "Scene.h"
#include "ECS/SystemManager/Systems/System/PickingSystem/PickingSystem.h"
#include "ImGuizmo/ImGuizmo.h"

namespace Eclipse
{
	void SceneWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&SceneWindow::RunMainWindow, this));
	}

	void SceneWindow::Init()
	{
		mViewportSize = glm::vec2{};
		mSceneBufferSize = glm::vec2{};
		Type = EditorWindowType::EWT_SCENE;
		WindowName = "Scene View " ICON_MDI_MONITOR;
		m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_SCENE);
	}

	void SceneWindow::Unload()
	{
	}

	void SceneWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();
		//std::cout << "Scene View: " << ImGui::GetWindowDockID() << std::endl;
		if (mViewportSize.getX() != viewportPanelSize.x ||
			mViewportSize.getY() != viewportPanelSize.y)
		{
			// Resize the framebuffer based on the size of the imgui window
			m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y), FrameBufferMode::FBM_SCENE);
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			engine->gFrameBufferManager->UpdateAspectRatio(FrameBufferMode::FBM_SCENE, mViewportSize);
		}

		ChildSettings settings;
		settings.Name = "SceneFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&SceneWindow::RunFrameBuffer, this));
		std::string dummy;
		engine->editorManager->DragAndDropInst_.StringPayloadTarget("prefab", dummy,
			"Created Prefab!", PayloadTargetType::PTT_WINDOW);
	}

	void SceneWindow::RunFrameBuffer()
	{
		// Update Frame Buffer Settings
		mSceneBufferSize = glm::vec2{ ECGui::GetWindowWidth(), ECGui::GetWindowHeight() };
		mSceneBufferPos = ECGui::GetWindowPos();
		mCursorScreenPos = ECGui::GetCursorScreenPos();

		// Set Image size
		ECGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// ImGuizmo Logic
		if (!engine->editorManager->IsEntityListEmpty() && m_GizmoType != -1)
			if (!engine->editorManager->IsAnySwitchWindowHovered())
				OnGizmoUpdateEvent();

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here when hovering on window
			// ImGuizmo Logic
			OnCameraMoveEvent();
			OnCameraZoomEvent();
			OnSelectEntityEvent();
		}

		if (ECGui::IsItemActive())
			IsWindowActive = true;
		else
			IsWindowActive = false;
	}

	void SceneWindow::OnGizmoUpdateEvent()
	{
		Entity selectedEntity = engine->editorManager->GetSelectedEntity();

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ECGui::GetWindowWidth();
		float windowHeight = (float)ECGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// Camera
		Entity cameraEntity = static_cast<Entity>(engine->gCamera.GetEditorCameraID());
		const auto& camCom = engine->world.GetComponent<CameraComponent>(cameraEntity);

		// Selected Entity Transform
		auto& transCom = engine->world.GetComponent<TransformComponent>(selectedEntity);
		glm::mat4 transform{};

		ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transCom.position.ConvertToGlmVec3Type()),
			glm::value_ptr(transCom.rotation.ConvertToGlmVec3Type()),
			glm::value_ptr(transCom.scale.ConvertToGlmVec3Type()),
			glm::value_ptr(transform));

		// Snapping
		glm::vec3 snapValues{};

		switch (m_GizmoType)
		{
		case ImGuizmo::OPERATION::TRANSLATE:
			snapValues =
			{ mSnapSettings.mPosSnapValue,
			  mSnapSettings.mPosSnapValue,
			  mSnapSettings.mPosSnapValue };
			/*{ mSnapSettings.mPosSnapValue,
			  mSnapSettings.mPosSnapValue,
			  mSnapSettings.mPosSnapValue };*/
			break;
		case ImGuizmo::OPERATION::ROTATE:
			snapValues =
			{ mSnapSettings.mRotSnapValue,
			  mSnapSettings.mRotSnapValue,
			  mSnapSettings.mRotSnapValue };
			/*{ mSnapSettings.mRotSnapValue,
			  mSnapSettings.mRotSnapValue,
			  mSnapSettings.mRotSnapValue };*/
			break;
		case ImGuizmo::OPERATION::SCALE:
			snapValues =
			{ mSnapSettings.mScaleSnapValue,
			  mSnapSettings.mScaleSnapValue,
			  mSnapSettings.mScaleSnapValue };
			/*{ mSnapSettings.mScaleSnapValue,
			  mSnapSettings.mScaleSnapValue,
			  mSnapSettings.mScaleSnapValue };*/
			break;
		default:
			break;
		}

		// ImGuiIO& io = ImGui::GetIO();

		ImGuizmo::Manipulate(glm::value_ptr(camCom.viewMtx), glm::value_ptr(camCom.projMtx),
			(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
			nullptr, IsSnapping ? glm::value_ptr(snapValues) : nullptr);

		if (ImGuizmo::IsUsing() && ECGui::IsItemHovered())
		{
			glm::vec3 translation, rotation, scale;
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation),
				glm::value_ptr(rotation), glm::value_ptr(scale));

			// glm::vec3 deltaRotation = rotation - transCom.rotation.ConvertToGlmVec3Type();

			auto& ent = engine->world.GetComponent<EntityComponent>(selectedEntity);

			switch (m_GizmoType)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				transCom.position = translation;
				CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.position, transCom.position, selectedEntity });			

				if (engine->world.CheckComponent<ChildComponent>(selectedEntity))
				{
					auto& child = engine->world.GetComponent<ChildComponent>(selectedEntity);
					child.UpdateChildren = true;
					/*std::cout << selectedEntity << ": " << lexical_cast<std::string>(child.UpdateChildren) << std::endl;*/

					if (ent.Tag != EntityType::ENT_MESH)
					{
						engine->gPicker.UpdateAabb(selectedEntity);
						engine->gDynamicAABBTree.UpdateData(selectedEntity);
					}
				}
				break;
			case ImGuizmo::OPERATION::ROTATE:
				transCom.rotation = rotation;
				CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.rotation, transCom.rotation, selectedEntity });
				break;
			case ImGuizmo::OPERATION::SCALE:
				transCom.scale = scale;
				CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.scale, transCom.scale, selectedEntity });
				break;
			default:
				break;
			}

			//Update for DynamicAABB Tree -Rachel
			engine->gPicker.UpdateAabb(selectedEntity);
			engine->gDynamicAABBTree.UpdateData(selectedEntity);
			if ((engine->world.CheckComponent<ParentComponent>(selectedEntity)) && (ent.Tag != EntityType::ENT_MODEL))
			{
				auto& parent = engine->world.GetComponent<ParentComponent>(selectedEntity);

				for (auto& it : parent.child)
				{
					auto& transform = engine->world.GetComponent<TransformComponent>(it);
				
					engine->gPicker.UpdateAabb(it);
					engine->gDynamicAABBTree.UpdateData(it);
				}
			}

			OnCopyEntityEvent();
		}
		else if (!ImGuizmo::IsUsing() && ImGui::IsMouseReleased(0)
			&& ECGui::IsItemHovered())
		{
			CommandHistory::DisableMergeForMostRecentCommand();

			if (engine->world.CheckComponent<ChildComponent>(selectedEntity))
			{
				auto& child = engine->world.GetComponent<ChildComponent>(selectedEntity);
				child.UpdateChildren = false;
				/*std::cout << selectedEntity << ": " << lexical_cast<std::string>(child.UpdateChildren) << std::endl;*/
			}
		}

		if (!ImGuizmo::IsUsing() && ImGui::IsMouseReleased(0) 
			&& !ImGui::IsMouseDragging(0))
		{
			IsCopying = false;
		}
	}

	void SceneWindow::OnCameraZoomEvent()
	{
		ImGuiIO& io = ImGui::GetIO();

		if (io.MouseWheel != 0.0f)
		{
			// ImGui Scroll Up Detection
			if (io.MouseWheel > 0.0f)
				engine->gCamera.GetInput().set(8, 1);
			else
				engine->gCamera.GetInput().set(8, 0);

			// ImGui Scroll Down Detection
			if (io.MouseWheel < 0.0f)
				engine->gCamera.GetInput().set(9, 1);
			else
				engine->gCamera.GetInput().set(9, 0);
		}
		else
		{
			engine->gCamera.GetInput().set(8, 0);
			engine->gCamera.GetInput().set(9, 0);
		}
	}

	void SceneWindow::OnSelectEntityEvent()
	{
		if (ECGui::IsMouseClicked(0) && !ImGuizmo::IsUsing())
		{
			engine->world.GetSystem<PickingSystem>()->EditorUpdate();

			if (engine->gPicker.GetCurrentCollisionID() != MAX_ENTITY)
				engine->editorManager->SetSelectedEntity(engine->gPicker.GetCurrentCollisionID());
		}
	}

	void SceneWindow::OnCopyEntityEvent()
	{
		if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_LEFTALT)))
		{
			if (!IsCopying)
			{
				if (engine->world.CheckComponent<ParentComponent>(engine->editorManager->GetSelectedEntity()))
				{
					IterativeCopy(engine->editorManager->GetSelectedEntity());
				}
				else
				{
					Entity newID = LoadEntity(engine->editorManager->GetSelectedEntity());

					if (engine->world.CheckComponent<ChildComponent>(newID))
					{
						auto& child = engine->world.GetComponent<ChildComponent>(newID);
						auto& parent = engine->world.GetComponent<ParentComponent>(child.parentIndex);
						parent.child.push_back(newID);
					}
				}
				
				IsCopying = true;
			}
		}
	}

	void SceneWindow::IterativeCopy(const Entity& ID)
	{
		std::queue<Entity> parentsQ;
		std::unordered_map<Entity, std::vector<Entity>> ParentToFamily;
		std::unordered_map<Entity, Entity> OldToNewEntity;
		parentsQ.push(ID);
		Entity MainParentID = LoadEntity(ID);
		OldToNewEntity[ID] = MainParentID;

		while (!parentsQ.empty())
		{
			Entity CurrID = parentsQ.front();
			parentsQ.pop();

			if (engine->world.CheckComponent<ParentComponent>(CurrID))
			{
				auto& parentCom = engine->world.GetComponent<ParentComponent>(ID);
				std::vector<Entity> List;

				for (const auto& childEnt : parentCom.child)
				{
					Entity newChildID = LoadEntity(childEnt);

					if (engine->world.CheckComponent<ParentComponent>(childEnt))
					{
						parentsQ.push(childEnt);
						OldToNewEntity[childEnt] = newChildID;
					}

					List.push_back(newChildID);
				}

				ParentToFamily[OldToNewEntity[CurrID]] = List;
			}
		}

		for (const auto& [parent, childList] : ParentToFamily)
		{
			auto& parentcom = engine->world.GetComponent<ParentComponent>(parent);
			parentcom.child.clear();

			for (const auto& child2 : childList)
			{
				parentcom.child.push_back(child2);

				auto& childcom = engine->world.GetComponent<ChildComponent>(child2);
				childcom.parentIndex = parent;
			}
		}
		
		engine->editorManager->SetSelectedEntity(MainParentID);
	}

	Entity SceneWindow::LoadEntity(const Entity& ID)
	{
		Entity newID = engine->world.CopyEntity(engine->world, ID, all_component_list);
		auto& trans = engine->world.GetComponent<TransformComponent>(newID);
		auto& ent = engine->world.GetComponent<EntityComponent>(newID);
		engine->gPicker.GenerateAabb(newID, trans, ent.Tag);
		engine->editorManager->RegisterExistingEntity(newID);

		return newID;
	}

	void SceneWindow::OnCameraMoveEvent()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 value_with_lock_threshold = ECGui::GetMouseDragDelta(1);
		const float benchmarkValue = 0.0f;

		// ImGui Right Click Detection
		if (ECGui::IsMouseDragging(1))
		{
			// Camera Yaw Right
			if (value_with_lock_threshold.x > benchmarkValue && io.MouseDelta.x > 0.0f)
				engine->gCamera.GetInput().set(7, 1);
			else
				engine->gCamera.GetInput().set(7, 0);

			// Camera Yaw Left
			if (value_with_lock_threshold.x < benchmarkValue && io.MouseDelta.x < 0.0f)
				engine->gCamera.GetInput().set(6, 1);
			else
				engine->gCamera.GetInput().set(6, 0);

			// Camera Pitch Down
			if (value_with_lock_threshold.y > benchmarkValue && io.MouseDelta.y > 0.0f)
				engine->gCamera.GetInput().set(5, 1);
			else
				engine->gCamera.GetInput().set(5, 0);

			// Camera Pitch Up
			if (value_with_lock_threshold.y < benchmarkValue && io.MouseDelta.y < 0.0f)
				engine->gCamera.GetInput().set(4, 1);
			else
				engine->gCamera.GetInput().set(4, 0);

			// Camera Move Front
			if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_W)))
				engine->gCamera.GetInput().set(2, 1);
			else
				engine->gCamera.GetInput().set(2, 0);

			// Camera Move Left
			if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_A)))
				engine->gCamera.GetInput().set(1, 1);
			else
				engine->gCamera.GetInput().set(1, 0);

			// Camera Move Back
			if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_S)))
				engine->gCamera.GetInput().set(3, 1);
			else
				engine->gCamera.GetInput().set(3, 0);

			// Camera Move Right
			if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_D)))
				engine->gCamera.GetInput().set(0, 1);
			else
				engine->gCamera.GetInput().set(0, 0);

			// Camera Move Up
			if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_Q)))
				engine->gCamera.GetInput().set(10, 1);
			else
				engine->gCamera.GetInput().set(10, 0);

			// Camera Move Down
			if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_E)))
				engine->gCamera.GetInput().set(11, 1);
			else
				engine->gCamera.GetInput().set(11, 0);
		}
		else
		{
			engine->gCamera.GetInput().set(0, 0);
			engine->gCamera.GetInput().set(1, 0);
			engine->gCamera.GetInput().set(2, 0);
			engine->gCamera.GetInput().set(3, 0);
			engine->gCamera.GetInput().set(4, 0);
			engine->gCamera.GetInput().set(5, 0);
			engine->gCamera.GetInput().set(6, 0);
			engine->gCamera.GetInput().set(7, 0);
			engine->gCamera.GetInput().set(10, 0);
			engine->gCamera.GetInput().set(11, 0);
		}
	}

	SnapValueSettings& SceneWindow::GetRefToSnapSettings()
	{
		return mSnapSettings;
	}

	SnapValueSettings SceneWindow::GetSnapSettings()
	{
		return mSnapSettings;
	}

	glm::vec2 SceneWindow::GetSceneBufferPos()
	{
		return mSceneBufferPos.ConvertToGlmVec2Type();
	}

	glm::vec2 SceneWindow::GetSceneBufferSize()
	{
		return mSceneBufferSize.ConvertToGlmVec2Type();
	}

	glm::vec2 SceneWindow::GetCursorScreenPos()
	{
		return mCursorScreenPos.ConvertToGlmVec2Type();
	}

	int SceneWindow::GetGizmoType() const
	{
		return m_GizmoType;
	}

	bool SceneWindow::GetIsWindowActive() const
	{
		return IsWindowActive;
	}

	bool SceneWindow::GetSnapping() const
	{
		return IsSnapping;
	}

	void SceneWindow::SetGizmoType(int type)
	{
		m_GizmoType = type;
	}

	void SceneWindow::SetSnapping(bool active)
	{
		IsSnapping = active;
	}
}
