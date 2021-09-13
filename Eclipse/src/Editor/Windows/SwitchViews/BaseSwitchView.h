#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class BaseSwitchViewWindow : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;

		void RunMainWindow();
		void RunFrameBuffer();
		void RenderSettingsHeader();
		void OnKeyPressedEvent();
		void OnGizmoUpdateEvent();
		void OnCameraZoomEvent();
		void OnCameraMoveEvent();
	protected:
		glm::vec2 mViewportSize;
		FrameBuffer* m_frameBuffer;
		int m_GizmoType{ 0 };
		std::vector<std::string> mProjectionView_List;
		bool IsWireframeMode{ false };
	};
}
#pragma once
