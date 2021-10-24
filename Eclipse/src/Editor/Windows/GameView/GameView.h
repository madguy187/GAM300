#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class eGameViewWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void RunMainWindow();
		void RunFrameBuffer();
		void SetFullScreenViewport();

		bool GetIsViewFullscreen();
		void SetViewToFullscreen(bool active);
		void SetViewToOriginalState(bool active);
	private:
		ECVec2 mViewportSize{ 0.f,0.f};
		FrameBuffer* m_frameBuffer = nullptr;
		unsigned int DockIDTracker{ 0 };
		bool IsViewFullscreen{ false };
		bool IsViewBackToOriginalState{ false };
		bool AlreadySetDockIDTracker{ false };
	};
}
