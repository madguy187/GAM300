#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class Scene : public ECGuiWindow
	{
	public:
		void Update() override;
		Scene();
	private:
	};
}
