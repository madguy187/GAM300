#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	struct SnapValueSettings
	{
		float mPosSnapValue{ 5.f };
		float mScaleSnapValue{ 1.f };
		float mRotSnapValue{ 45.f };
	};

	class SceneWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		SceneWindow();
		void RunMainWindow();
		void RunFrameBuffer();
		void OnKeyPressedEvent();
		void OnGizmoUpdateEvent();
		void OnCameraMoveEvent();
		void OnCameraZoomEvent();
		void OnSelectEntityEvent();

		SnapValueSettings& GetRefToSnapSettings();
		SnapValueSettings GetSnapSettings();
		glm::vec2 GetSceneBufferPos();
		glm::vec2 GetSceneBufferSize();
		glm::vec2 GetCursorScreenPos();
	private:
		glm::vec2 mViewportSize;
		glm::vec2 mSceneBufferSize;
		ECVec2 mSceneBufferPos;
		ECVec2 mCursorScreenPos;
		SnapValueSettings mSnapSettings;
		// Hold Frame Buffer Object
		FrameBuffer* m_frameBuffer;
		// Hold Gizmo Type
		int m_GizmoType{ 0 };
	};
}
