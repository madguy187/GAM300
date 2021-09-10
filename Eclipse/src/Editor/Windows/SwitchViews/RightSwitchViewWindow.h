#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class RightSwitchViewWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		RightSwitchViewWindow();
		void RunMainWindow();
		void RunFrameBuffer();
	private:
		glm::vec2 mViewportSize;
		// Hold Frame Buffer Objects
		std::shared_ptr<FrameBuffer> m_frameBuffer_RIGHT;
	};
}
#pragma once
