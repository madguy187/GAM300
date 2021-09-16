#include "pch.h"
#include "Scene.h"
#include "ECS/SystemManager/Systems/System/PickingSystem.h"
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
		WindowName = "Scene";
		m_frameBuffer = engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SCENEVIEW);
	}

	void SceneWindow::Unload()
	{
	}

	void SceneWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "SceneFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&SceneWindow::RunFrameBuffer, this));
	}

	void SceneWindow::RunFrameBuffer()
	{
		// Update Frame Buffer Settings
		mSceneBufferSize = glm::vec2{ ECGui::GetWindowWidth(), ECGui::GetWindowHeight() };
		mSceneBufferPos = ECGui::GetWindowPos();
		mCursorScreenPos = ECGui::GetCursorScreenPos();

		/*std::cout << "SceneBuffer Size: " << mSceneBufferSize.x << " " << mSceneBufferSize.y << std::endl;*/
		/*std::cout << "SceneBuffer Pos: " << mSceneBufferPos.x << " " << mSceneBufferPos.y << std::endl;
		std::cout << "CursorScreen Pos: " << mCursorScreenPos.x << " " << mCursorScreenPos.y << std::endl;*/
		RenderSceneHeader();

		// Set Image size
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y * 0.93f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		//// ImGuizmo Logic
		if (!engine->editorManager->IsEntityListEmpty() && m_GizmoType != -1)
		{
			OnGizmoUpdateEvent();
		}
		
		if (ECGui::IsItemHovered() /*&& ImGui::IsWindowFocused() *//*temp fix*/)
		{
			// Do all the future stuff here when hovering on window
			// ImGuizmo Logic
			OnKeyPressedEvent();
			OnCameraMoveEvent();
			OnCameraZoomEvent();
			OnSelectEntityEvent();
		}
	}

	void SceneWindow::OnKeyPressedEvent()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Gizmos
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Q)))
		{
			if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
				m_GizmoType = -1;
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W)))
		{
			if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_E)))
		{
			if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R)))
		{
			if (!ImGuizmo::IsUsing() && !ImGui::IsMouseDown(1))
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		}
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
			break;
		case ImGuizmo::OPERATION::ROTATE:
			snapValues =
			{ mSnapSettings.mRotSnapValue,
			  mSnapSettings.mRotSnapValue,
			  mSnapSettings.mRotSnapValue };
			break;
		case ImGuizmo::OPERATION::SCALE:
			snapValues =
			{ mSnapSettings.mScaleSnapValue,
			  mSnapSettings.mScaleSnapValue,
			  mSnapSettings.mScaleSnapValue };
			break;
		default:
			break;
		}

		ImGuiIO& io = ImGui::GetIO();

		ImGuizmo::Manipulate(glm::value_ptr(camCom.viewMtx), glm::value_ptr(camCom.projMtx),
			(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
			nullptr, io.KeyCtrl ? glm::value_ptr(snapValues) : nullptr);

		/*static const float identityMatrix[16] =
		{ 1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f };

		ImGuizmo::DrawGrid(glm::value_ptr(camCom.viewMtx), glm::value_ptr(camCom.projMtx), identityMatrix, 100.f);*/

		if (ImGuizmo::IsUsing() && ECGui::IsItemHovered())
		{
			glm::vec3 translation, rotation, scale;
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation),
				glm::value_ptr(rotation), glm::value_ptr(scale));

			glm::vec3 deltaRotation = rotation - transCom.rotation.ConvertToGlmVec3Type();

			switch (m_GizmoType)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				transCom.position = translation;
				CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.position, transCom.position });
				break;
			case ImGuizmo::OPERATION::ROTATE:
				transCom.rotation += deltaRotation;
				CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.rotation, transCom.rotation });
				break;
			case ImGuizmo::OPERATION::SCALE:
				transCom.scale = scale;
				CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.scale, transCom.scale });
				break;
			default:
				break;
			}
		}
		else if (ImGuizmo::IsOver() && ImGui::IsMouseReleased(0))
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}

		/*ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::ViewManipulate(const_cast<float*>(glm::value_ptr(camCom.viewMtx)), camCom.fov, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);*/
	}

	void SceneWindow::OnCameraZoomEvent()
	{
		ImGuiIO& io = ImGui::GetIO();

		if (io.MouseWheel != 0.0f/* && io.KeyCtrl*/)
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
		if (ImGui::IsMouseClicked(0))
		{
			auto& picksys = engine->world.GetSystem<PickingSystem>();
			picksys->EditorUpdate();

			if (engine->gPicker.GetCurrentCollisionID() != MAX_ENTITY)
				engine->editorManager->SetSelectedEntity(engine->gPicker.GetCurrentCollisionID());
		}
	}

	void SceneWindow::RenderSceneHeader()
	{
		ECGui::InsertSameLine(mViewportSize.x / 2.35f);
		if (ECGui::ButtonBool("Play " ICON_FA_PLAY)) 
		{
			if (engine->GetPlayState())
			{
				engine->SetPlayState(false);
			}
			else
			{
				engine->SetPlayState(true);
			}
		}

		ECGui::InsertSameLine();
		if (ECGui::ButtonBool("Pause " ICON_FA_PAUSE)) 
		{
			if (engine->GetPauseState())
			{
				engine->SetPauseState(false);
			}
			else
			{
				engine->SetPauseState(true);
			}
		}

		ECGui::InsertSameLine();
		if (ECGui::ButtonBool("Step " ICON_FA_STEP_FORWARD))
		{
			if (engine->GetStepState())
			{
				engine->SetStepState(false);
			}
			else
			{
				engine->SetStepState(true);
			}
		}
	}

	void SceneWindow::OnCameraMoveEvent()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(1);
		const float benchmarkValue = 0.0f;

		// ImGui Right Click Detection
		if (ImGui::IsMouseDragging(1))
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
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_W)))
				engine->gCamera.GetInput().set(2, 1);
			else
				engine->gCamera.GetInput().set(2, 0);

			// Camera Move Left
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
				engine->gCamera.GetInput().set(1, 1);
			else
				engine->gCamera.GetInput().set(1, 0);

			// Camera Move Back
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S)))
				engine->gCamera.GetInput().set(3, 1);
			else
				engine->gCamera.GetInput().set(3, 0);

			// Camera Move Right
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
				engine->gCamera.GetInput().set(0, 1);
			else
				engine->gCamera.GetInput().set(0, 0);

			// Camera Move Up
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Q)))
				engine->gCamera.GetInput().set(10, 1);
			else
				engine->gCamera.GetInput().set(10, 0);

			// Camera Move Down
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_E)))
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
		return mSceneBufferSize;
	}

	glm::vec2 SceneWindow::GetCursorScreenPos()
	{
		return mCursorScreenPos.ConvertToGlmVec2Type();
	}
}
