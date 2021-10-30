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
		void OnGizmoUpdateEvent(int GizmoType);
		void OnCameraZoomEvent();
		void OnCameraMoveEvent();
		bool GetIsWindowActive() const;
		bool GetIsWindowHovered() const;
	protected:
		CameraComponent::CameraType mCamType;
		ECVec2 mViewportSize;
		FrameBuffer* m_frameBuffer;
		FrameBufferMode FBO_Mode;
		std::vector<std::string> mProjectionView_List;
		bool IsWireframeMode{ false };
		bool IsWindowActive{ false };
		bool IsWindowHovering{ false };
	};
}
#pragma once
