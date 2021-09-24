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
		void RunFrameBuffer(int GizmoType);
		void RenderSettingsHeader();
		void OnKeyPressedEvent();
		void OnGizmoUpdateEvent(int GizmoType);
		void OnCameraZoomEvent();
		void OnCameraMoveEvent();
		bool GetIsWindowActive();
	protected:
		glm::vec2 mViewportSize;
		FrameBuffer* m_frameBuffer;
		std::vector<std::string> mProjectionView_List;
		bool IsWireframeMode{ false };
		bool IsWindowActive{ false };
	};
}
#pragma once
