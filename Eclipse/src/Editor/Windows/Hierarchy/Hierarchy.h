#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class Hierarchy final : public ECGuiWindow
	{
	public:
		void Update() override;
		Hierarchy();
		void DrawImpl();
	private:
	};
}
