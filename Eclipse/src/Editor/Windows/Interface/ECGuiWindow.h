#pragma once

namespace Eclipse
{
	class ECGuiWindow
	{
	public:
		virtual void Init() = 0;
		virtual void DrawImpl() = 0;
		virtual ~ECGuiWindow() = default;

		bool isVisible{ true };
	};
}
