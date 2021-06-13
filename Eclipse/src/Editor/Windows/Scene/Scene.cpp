#include "pch.h"
#include "Scene.h"
#include "ImGuizmo.h"

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

		// ImGuizmo Logic
		if (!engine->editorManager->EntityHierarchyList_.empty())
		{
			static const float identityMatrix[16] =
			{ 1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f };
			Entity selectedEntity = engine->editorManager->GetSelectedEntity();

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ECGui::GetWindowWidth();
			float windowHeight = (float)ECGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			Entity cameraEntity = static_cast<Entity>(engine->gCamera.GetEditorCameraID());
			const auto& camCom = engine->world.GetComponent<CameraComponent>(cameraEntity);
			//engine->gCamera.ComputePerspectiveMtx(*camCom);
			glm::mat4 cameraProjection = camCom.projMtx;
			glm::mat4 cameraView = glm::inverse(engine->world.GetComponent<TransformComponent>(cameraEntity).GetTransform());

			// Selected Entity Transform
			auto& transCom = engine->world.GetComponent<TransformComponent>(selectedEntity);
			glm::mat4 transform = transCom.GetTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transform));

			/*ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), identityMatrix, 100.f);*/

			if (ImGuizmo::IsUsing())
			{
				transCom.position.setX(glm::vec3(transform[3]).x);
				transCom.position.setY(glm::vec3(transform[3]).y);
				transCom.position.setZ(glm::vec3(transform[3]).z);
			}

			std::cout << ImGuizmo::IsOver() << std::endl;
		}

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here when hovering on window
		}
	}
}
