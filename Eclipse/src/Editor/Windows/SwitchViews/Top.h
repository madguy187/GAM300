#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class eTopViewWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		eTopViewWindow();
		void InitilializeFrameBuffer();
		void RunFrameBuffer();
	private:
		glm::vec2 mViewportSize;
		// Hold Frame Buffer Object
		std::shared_ptr<FrameBuffer> m_frameBuffer;
	};
}
