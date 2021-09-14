#pragma once

#include "BaseSwitchView.h"

namespace Eclipse
{
	class BottomSwitchViewWindow final : public BaseSwitchViewWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
	};
}
