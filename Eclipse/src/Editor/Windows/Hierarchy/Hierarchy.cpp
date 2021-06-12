#include "pch.h"
#include "Hierarchy.h"

namespace Eclipse
{
	void Hierarchy::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&Hierarchy::DrawImpl, this));
	}

	Hierarchy::Hierarchy()
	{
		Type = EditorWindowType::HIERARCHY;
		WindowName = "Hierarchy";
	}

	void Hierarchy::DrawImpl()
	{

	}
}