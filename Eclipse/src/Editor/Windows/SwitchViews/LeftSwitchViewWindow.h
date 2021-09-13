#pragma once

#include "BaseSwitchView.h"

namespace Eclipse
{
	class LeftSwitchViewWindow final : public BaseSwitchViewWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
	};
}
#pragma once
