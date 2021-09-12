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
		//engine->gCamera.GetCameraID(CameraComponent::CameraType::);
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
		// TOP RIGHT
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer_TOP->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (!engine->editorManager->IsEntityListEmpty() && m_GizmoType != -1)
		{
			OnGizmoUpdateEvent();
		}
		
		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
			// ImGuizmo Logic
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

			//Math::DecomposeTransform(transform, translation, rotation, scale);

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
		else if (ImGuizmo::IsOver())
		{
			CommandHistory::DisableMergeForMostRecentCommand();
		}
	}
}
