#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class Inspector : public ECGuiWindow
	{
	public:
		void Update() override;
		Inspector();
		void DrawImpl();
	private:
	};
}
