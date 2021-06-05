#pragma once

#include "Global.h"

namespace Eclipse
{
	class ECGuiWindow
	{
	public:
		virtual void Update() = 0;
		virtual ~ECGuiWindow() = default;

		EditorWindowType Type{ EditorWindowType::UNASSIGNED };
		const char* WindowName{ nullptr };
		bool IsVisible{ true };
	};
}
