#include "pch.h"
#include "Scene.h"

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
	}

	void Scene::InitilializeFrameBuffer()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			// Resize the framebuffer based on the size of the imgui window
			//m_frameBuffer->OnResize(static_cast<GLuint>(viewportPanelSize.x), static_cast<GLuint>(viewportPanelSize.y));
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		ChildSettings settings;
		settings.Name = "SceneFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&Scene::RunFrameBuffer, this));
	}

	void Scene::RunFrameBuffer()
	{
		// Have an id for color attachment
		//GLuint textureID = m_frameBuffer->GetColourAttachment();

		// Set Image size
		//ImGui::Image((void*)(static_cast<size_t>(textureID)), ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ECGui::IsItemHovered())
		{
			// Do all the future stuff here
		}
	}
}
