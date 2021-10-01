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
		void SnappingManager(bool& isActive);
		//void SetPosSnap();
		//void SetRotSnap();
		//void SetScaleSnap();
		void Translate();
		void Rotate();
		void Scale();
		void HideTabBar(bool active);

	private:
		bool IsTabBarHidden{ false };
	};
}
