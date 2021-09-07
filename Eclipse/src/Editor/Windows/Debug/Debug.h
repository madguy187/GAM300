#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class DebugWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		DebugWindow();
		void DrawImpl();
	private:
	};
}
