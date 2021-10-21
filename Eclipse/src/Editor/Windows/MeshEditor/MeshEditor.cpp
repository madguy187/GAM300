#include "pch.h"
#include "MeshEditor.h"

namespace Eclipse
{
	void MeshEditorWindow::Update()
	{
		if (IsVisible)
		{
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&MeshEditorWindow::RunMainWindow, this));
			ECGui::DrawMainWindow<void()>("Mesh Settings", std::bind(&MeshEditorWindow::RunMeshSettings, this));
		}
	}

	void MeshEditorWindow::Init()
	{
		Type = EditorWindowType::EWT_MESHEDITOR;
		WindowName = "Mesh Editor";
		m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_MESHEDITOR);
		//IsVisible = true;
	}

	void MeshEditorWindow::Unload()
	{}

	void MeshEditorWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize.getX() != viewportPanelSize.x ||
			mViewportSize.getY() != viewportPanelSize.y)
		{
			m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), 
				static_cast<unsigned>(viewportPanelSize.y), FrameBufferMode::FBM_MESHEDITOR);
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			engine->gFrameBufferManager->UpdateAspectRatio(FrameBufferMode::FBM_MESHEDITOR, mViewportSize);
		}

		ChildSettings settings;
		settings.Name = "MeshEditorFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&MeshEditorWindow::RunFrameBuffer, this));
	}

	void MeshEditorWindow::RunFrameBuffer()
	{
		ECGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	void MeshEditorWindow::RunMeshSettings()
	{

	}
}