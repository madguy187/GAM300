#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class HeaderWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void RunPlayPauseStep();
		void HideTabBar(bool active);
	private:
		bool IsTabBarHidden{ false };
	};
}
