#include "pch.h"
#include "Scene.h"
#include "ImGuizmo.h"
#include "Library/Math/Math.h"

namespace Eclipse
{
	void SceneWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&SceneWindow::InitilializeFrameBuffer, this));
	}

	SceneWindow::SceneWindow() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::SCENE;
		WindowName = "Scene";

		m_frameBuffer = std::make_shared<FrameBuffer>(*engine->gGraphics.mRenderContext.GetFramebuffer(FrameBufferMode::SCENEVIEW));
	}

	void SceneWindow::InitilializeFrameBuffer()
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
		// Set Image size
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// ImGuizmo Logic
		if (!engine->editorManager->EntityHierarchyList_.empty() && m_GizmoType != -1)
		{
			GizmoUpdate();
		}

		if (ECGui::IsItemHovered() && ImGui::IsWindowFocused() /*temp fix*/)
		{
			// Do all the future stuff here when hovering on window
			OnKeyPressed();
		}
	}

	void SceneWindow::OnKeyPressed()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Delete Entity
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
		{
			if (!engine->editorManager->EntityHierarchyList_.empty())
			{
				Entity currEnt = engine->editorManager->GetSelectedEntity();

				if (currEnt != engine->gCamera.GetEditorCameraID() ||
					currEnt != engine->gCamera.GetGameCameraID())
				{
					engine->editorManager->DestroyEntity(currEnt);
				}
			}
		}
		// Gizmos
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Q)))
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = -1;
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W)))
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_E)))
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R)))
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		}
	}

	void SceneWindow::GizmoUpdate()
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

		if (ImGuizmo::IsUsing())
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
				break;
			case ImGuizmo::OPERATION::ROTATE:
				transCom.rotation += deltaRotation;
				break;
			case ImGuizmo::OPERATION::SCALE:
				transCom.scale = scale;
				break;
			default:
				break;
			}
		}

		/*ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::ViewManipulate(const_cast<float*>(glm::value_ptr(camCom.viewMtx)), camCom.fov, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);*/
	}

	SnapValueSettings& SceneWindow::GetSnapSettings()
	{
		return mSnapSettings;
	}
}
