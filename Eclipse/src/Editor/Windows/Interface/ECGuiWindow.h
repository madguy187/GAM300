#pragma once

#include "Global.h"

namespace Eclipse
{
	class ECGuiWindow
	{
	public:
		virtual void Update() = 0;
		virtual void Init() = 0;
		virtual void Unload() = 0;
		virtual ~ECGuiWindow() = default;

		EditorWindowType Type{ EditorWindowType::EWT_UNASSIGNED };
		const char* WindowName{ nullptr };
		bool IsVisible{ true };
	};
}
