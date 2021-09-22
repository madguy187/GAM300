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
		void RenderGameHeader();
	private:
		glm::vec2 mViewportSize;
		// Hold Frame Buffer Object
		std::shared_ptr<FrameBuffer> m_frameBuffer;
	};
}
