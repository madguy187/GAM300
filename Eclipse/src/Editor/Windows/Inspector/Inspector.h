#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class Inspector final : public ECGuiWindow
	{
	public:
		void Update() override;
		Inspector();
		void DrawImpl();
	private:
	};
}
