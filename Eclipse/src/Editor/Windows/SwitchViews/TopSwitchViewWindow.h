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
		void RenderSettingsHeader();
		void OnKeyPressedEvent();
		void OnGizmoUpdateEvent();
		void OnCameraZoomEvent();
		void OnCameraMoveEvent();
	private:
		glm::vec2 mViewportSize;
		std::shared_ptr<FrameBuffer> m_frameBuffer_TOP;
		int m_GizmoType{ 0 };
		/*************************************
		           0  ->  Ortho
				   1  ->  Persp
		**************************************/
		std::bitset<2> mProjectionView_Bits;
		std::vector<std::string> mProjectionView_List;
		bool IsWireframeMode{ false };
	};
}
