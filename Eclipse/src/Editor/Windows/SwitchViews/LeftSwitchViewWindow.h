#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class LeftSwitchViewWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		LeftSwitchViewWindow();
		void RunMainWindow();
		void RunFrameBuffer();
	private:
		glm::vec2 mViewportSize;
		// Hold Frame Buffer Objects
		std::shared_ptr<FrameBuffer> m_frameBuffer_LEFT;
	};
}
#pragma once
