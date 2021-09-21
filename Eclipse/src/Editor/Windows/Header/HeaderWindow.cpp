#include "pch.h"
#include "HeaderWindow.h"

namespace Eclipse
{
	void HeaderWindow::Update()
	{

	}

	void HeaderWindow::Init()
	{
		Type = EditorWindowType::EWT_HEADER;
		WindowName = "Header";
		IsVisible = true;
	}

	void HeaderWindow::Unload()
	{}
}