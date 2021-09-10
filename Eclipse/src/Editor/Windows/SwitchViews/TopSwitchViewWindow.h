#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class TopSwitchViewWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		TopSwitchViewWindow();
		void RunMainWindow();
		void RunFrameBuffer();
		void OnGizmoUpdateEvent();
	private:
		glm::vec2 mViewportSize;
		// Hold Frame Buffer Objects
		std::shared_ptr<FrameBuffer> m_frameBuffer_TOP;
		int m_GizmoType{ 0 };
	};
}
