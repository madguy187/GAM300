#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class eGameViewWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		eGameViewWindow();
		void RunMainWindow();
		void RunFrameBuffer();
	private:
		glm::vec2 mViewportSize;
		// Hold Frame Buffer Object
		std::shared_ptr<FrameBuffer> m_frameBuffer;
	};
}
