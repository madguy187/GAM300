#include "pch.h"
#include "MaterialEditor.h"

namespace Eclipse
{
	void MaterialEditorWindow::Update()
	{
		if (IsVisible)
		{
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&MaterialEditorWindow::RunMainWindow, this));
			ECGui::DrawMainWindow<void()>("Material Settings", std::bind(&MaterialEditorWindow::RunMaterialSettings, this));
		}
	}

	void MaterialEditorWindow::Init()
	{
		Type = EditorWindowType::EWT_MATERIALEDITOR;
		WindowName = "Material Editor";
		m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_MATERIALEDITOR);
		IsVisible = false;
	}

	void MaterialEditorWindow::Unload()
	{
	}

	void MaterialEditorWindow::RunMainWindow()
	{
		ImVec2 viewportPanelSize = ECGui::GetWindowSize();

		if (mViewportSize.getX() != viewportPanelSize.x ||
			mViewportSize.getY() != viewportPanelSize.y)
		{
			m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x),
				static_cast<unsigned>(viewportPanelSize.y), FrameBufferMode::FBM_MATERIALEDITOR);
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			engine->gFrameBufferManager->UpdateAspectRatio(FrameBufferMode::FBM_MATERIALEDITOR, mViewportSize);
		}

		ChildSettings settings;
		settings.Name = "MaterialEditorFrameBuffer";
		settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
		ECGui::DrawChildWindow<void()>(settings, std::bind(&MaterialEditorWindow::RunFrameBuffer, this));
	}

	void MaterialEditorWindow::RunFrameBuffer()
	{
		ECGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
			ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	void MaterialEditorWindow::RunMaterialSettings()
	{
	}
}