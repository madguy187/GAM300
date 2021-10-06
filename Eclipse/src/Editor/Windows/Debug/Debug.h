#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class DebugWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();
	private:
		void tempwindow();
	};
}
