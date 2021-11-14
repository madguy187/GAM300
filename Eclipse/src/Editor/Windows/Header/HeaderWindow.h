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
		void UtilitiesButtons();
		void CameraSetting();
		void SnappingManager();
		void Translate();
		void Rotate();
		void Scale();
		void HideTabBar(bool active);
		void Terminate();
	private:
		bool IsTabBarHidden{ false };
	};
}
