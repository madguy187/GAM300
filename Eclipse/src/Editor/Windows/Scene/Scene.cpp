#include "pch.h"
#include "Scene.h"
#include "ImGuizmo.h"
#include "Library/Math/Math.h"

namespace Eclipse
{
	void Scene::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&Scene::InitilializeFrameBuffer, this));
	}

	Scene::Scene() :
		mViewportSize{ 0.0f, 0.0f }
	{
		Type = EditorWindowType::SCENE;
		WindowName = "Scene";

		m_frameBuffer = std::make_shared<FrameBuffer>(*engine->gGraphics.mRenderContext.GetFramebuffer(FrameBufferMode::SCENEVIEW));
	}

	void Scene::InitilializeFrameBuffer()
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
		ECGui::DrawChildWindow<void()>(settings, std::bind(&Scene::RunFrameBuffer, this));
	}

	void Scene::RunFrameBuffer()
	{
		// Set Image size
		ImGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		m_GizmoType = ImGuizmo::OPERATION::ROTATE;

		// ImGuizmo Logic
		if (!engine->editorManager->EntityHierarchyList_.empty() && m_GizmoType != -1)
		{
			Entity selectedEntity = engine->editorManager->GetSelectedEntity();

			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ECGui::GetWindowWidth();
			float windowHeight = (float)ECGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			Entity cameraEntity = static_cast<Entity>(engine->gCamera.GetEditorCameraID());
			const auto& camCom = engine->world.GetComponent<CameraComponent>(cameraEntity);

			// Selected Entity Transform
			auto& transCom = engine->world.GetComponent<TransformComponent>(selectedEntity);
			glm::mat4 transform = transCom.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(camCom.viewMtx), glm::value_ptr(camCom.projMtx),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

			/*ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), identityMatrix, 100.f);*/

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - transCom.rotation.ConvertToGlmVec3Type();

				transCom.position = translation;
				transCom.rotation += deltaRotation;
				transCom.scale = scale;
			}

			/*std::cout << ImGuizmo::IsOver() << std::endl;*/
		}

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here when hovering on window
		}
	}
}
