#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class BottomSwitchViewWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		BottomSwitchViewWindow();
		void RunMainWindow();
		void RunFrameBuffer();
		void RenderSettingsHeader();
		void OnKeyPressedEvent();
		void OnGizmoUpdateEvent();
		void OnCameraZoomEvent();
		void OnCameraMoveEvent();
	private:
		glm::vec2 mViewportSize;
		FrameBuffer* m_frameBuffer_BOTTOM;
		int m_GizmoType{ 0 };
		std::vector<std::string> mProjectionView_List;
		bool IsWireframeMode{ false };
	};
}
