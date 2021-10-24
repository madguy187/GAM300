#include "pch.h"
#include "MaterialEditor.h"

namespace Eclipse
{
	void MaterialEditorWindow::Update()
	{
		if (IsVisible)
		{
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&MaterialEditorWindow::RunMainWindow, this));
			ECGui::DrawMainWindow<void()>("Mesh Settings", std::bind(&MaterialEditorWindow::RunMaterialSettings, this));
		}
	}

	void MaterialEditorWindow::Init()
	{
	}

	void MaterialEditorWindow::Unload()
	{
	}

	void MaterialEditorWindow::RunMainWindow()
	{
	}

	void MaterialEditorWindow::RunFrameBuffer()
	{
	}

	void MaterialEditorWindow::RunMaterialSettings()
	{
	}
}