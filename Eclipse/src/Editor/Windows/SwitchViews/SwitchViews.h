#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class SwitchViewsWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		SwitchViewsWindow();
		void RunMainWindow();
		void RunFrameBuffer();
	private:
		glm::vec2 mViewportSize;
		// Hold Frame Buffer Objects
		std::shared_ptr<FrameBuffer> m_frameBuffer_TOP;
		std::shared_ptr<FrameBuffer> m_frameBuffer_LEFT;
		std::shared_ptr<FrameBuffer> m_frameBuffer_RIGHT;
		std::shared_ptr<FrameBuffer> m_frameBuffer_BOTTOM;
	};
}
