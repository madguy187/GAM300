#include "pch.h"
#include "Inspector.h"

namespace Eclipse
{
	void Inspector::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&Inspector::DrawImpl, this));
	}

	Inspector::Inspector()
	{
		Type = EditorWindowType::INSPECTOR;
		WindowName = "Inspector";
	}

	void Inspector::DrawImpl()
	{
		// Do stuff here
	}
}