#include "pch.h"
#include "TopSwitchViewWindow.h"
#include "Editor/Windows/Scene/SceneView.h"

namespace Eclipse
{
	void TopSwitchViewWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&TopSwitchViewWindow::RunMainWindow, this));
	}

	void TopSwitchViewWindow::Unload()
	{
	}

	TopSwitchViewWindow::TopSwitchViewWindow() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::EWT_SWITCHVIEW_TOP;
		WindowName = "Top Switch View";
		IsVisible = false;

		m_frameBuffer_TOP = std::make_shared<FrameBuffer>
			(*engine->GraphicsManager.mRenderContext.GetFramebuffer(FrameBufferMode::SWITCHINGVIEWS_TOP));

		mProjectionView_List.push_back("Orthographic");
		mProjectionView_List.push_back("Perspective");
	}

	void TopSwitchViewWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "TopSwitchViewFrameBuffers";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&TopSwitchViewWindow::RunFrameBuffer, this));
	}

	void TopSwitchViewWindow::RunFrameBuffer()
	{
		RenderSettingsHeader();
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_TOP->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (!engine->editorManager->IsEntityListEmpty() && m_GizmoType != -1)
		{
			OnGizmoUpdateEvent();
			/*m_frameBuffer_TOP->SetRenderMode(FrameBuffer::RenderMode::Fill_Mode);*/
			//engine->gCamera.SetViewCameraProjectionType()
		}
		
		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
			OnKeyPressedEvent();
			OnCameraZoomEvent();
			OnCameraMoveEvent();
		}
	}

	void TopSwitchViewWindow::RenderSettingsHeader()
	{
		static size_t projIndex = 0;
		ComboListSettings settings{ "ProjectionComboList" };
		ECGui::CreateComboList(settings, mProjectionView_List, projIndex);

		if (projIndex == static_cast<size_t>(CameraComponent::ProjectionType::Perspective))
		{
			mProjectionView_Bits.set(0, 0);
			mProjectionView_Bits.set(1, 1);
			engine->gCamera.SetViewCameraProjectionType(CameraComponent::CameraType::TopView_Camera,
				CameraComponent::ProjectionType::Perspective);
		}
		else
		{
			mProjectionView_Bits.set(1, 0);
			mProjectionView_Bits.set(0, 1);
			engine->gCamera.SetViewCameraProjectionType(CameraComponent::CameraType::TopView_Camera,
				CameraComponent::ProjectionType::Orthographic);
		}

		ECGui::InsertSameLine();
		ECGui::CheckBoxBool("Wireframe", &IsWireframeMode, false);

		if (ImGui::IsItemDeactivated())
		{
			if (IsWireframeMode)
				m_frameBuffer_TOP->SetRenderMode(FrameBuffer::RenderMode::Wireframe_Mode);
			else
				m_frameBuffer_TOP->SetRenderMode(FrameBuffer::RenderMode::Fill_Mode);
		}
	}

	void TopSwitchViewWindow::OnKeyPressedEvent()
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

	void TopSwitchViewWindow::OnGizmoUpdateEvent()
	{
		Entity selectedEntity = engine->editorManager->GetSelectedEntity();

		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ECGui::GetWindowWidth();
		float windowHeight = (float)ECGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// Camera
		Entity cameraEntity = static_cast<Entity>(engine->gCamera.GetCameraID(CameraComponent::CameraType::TopView_Camera));
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
			{ engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mPosSnapValue,
			  engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mPosSnapValue,
			  engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mPosSnapValue };
			break;
		case ImGuizmo::OPERATION::ROTATE:
			snapValues =
			{ engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mRotSnapValue,
			  engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mRotSnapValue,
			  engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mRotSnapValue };
			break;
		case ImGuizmo::OPERATION::SCALE:
			snapValues =
			{ engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mScaleSnapValue,
			  engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mScaleSnapValue,
			  engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mScaleSnapValue };
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
	}

	void TopSwitchViewWindow::OnCameraZoomEvent()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Perspective
		if (mProjectionView_Bits.test(1))
		{
			if (io.MouseWheel != 0.0f)
			{
				// ImGui Scroll Up Detection
				if (io.MouseWheel > 0.0f)
					engine->gCamera.GetViewInput().set(6, 1);
				else
					engine->gCamera.GetViewInput().set(6, 0);

				// ImGui Scroll Down Detection
				if (io.MouseWheel < 0.0f)
					engine->gCamera.GetViewInput().set(7, 1);
				else
					engine->gCamera.GetViewInput().set(7, 0);
			}
			else
			{
				engine->gCamera.GetViewInput().set(6, 0);
				engine->gCamera.GetViewInput().set(7, 0);
			}
		}
		// Orthographic
		else if (mProjectionView_Bits.test(0))
		{
			if (io.MouseWheel != 0.0f)
			{
				// ImGui Scroll Up Detection
				if (io.MouseWheel > 0.0f)
					engine->gCamera.GetViewInput().set(4, 1);
				else
					engine->gCamera.GetViewInput().set(4, 0);

				// ImGui Scroll Down Detection
				if (io.MouseWheel < 0.0f)
					engine->gCamera.GetViewInput().set(5, 1);
				else
					engine->gCamera.GetViewInput().set(5, 0);
			}
			else
			{
				engine->gCamera.GetViewInput().set(4, 0);
				engine->gCamera.GetViewInput().set(5, 0);
			}
		}
	}

	void TopSwitchViewWindow::OnCameraMoveEvent()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(1);
		const float benchmarkValue = 0.0f;

		// ImGui Right Click Detection
		if (ImGui::IsMouseDragging(1))
		{
			// Camera Move Right
			if (value_with_lock_threshold.x > benchmarkValue && io.MouseDelta.x > 0.0f)
				engine->gCamera.GetViewInput().set(0, 1);
			else
				engine->gCamera.GetViewInput().set(0, 0);

			// Camera Move Left
			if (value_with_lock_threshold.x < benchmarkValue && io.MouseDelta.x < 0.0f)
				engine->gCamera.GetViewInput().set(1, 1);
			else
				engine->gCamera.GetViewInput().set(1, 0);

			// Camera Move Down
			if (value_with_lock_threshold.y > benchmarkValue && io.MouseDelta.y > 0.0f)
				engine->gCamera.GetViewInput().set(2, 1);
			else
				engine->gCamera.GetViewInput().set(2, 0);

			// Camera Move Up
			if (value_with_lock_threshold.y < benchmarkValue && io.MouseDelta.y < 0.0f)
				engine->gCamera.GetViewInput().set(3, 1);
			else
				engine->gCamera.GetViewInput().set(3, 0);
		}
		else
		{
			engine->gCamera.GetViewInput().set(0, 0);
			engine->gCamera.GetViewInput().set(1, 0);
			engine->gCamera.GetViewInput().set(2, 0);
			engine->gCamera.GetViewInput().set(3, 0);
		}
	}
}
