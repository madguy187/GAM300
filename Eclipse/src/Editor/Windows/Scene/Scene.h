#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	struct SnapValueSettings
	{
		float mPosSnapValue{ 1.f };
		float mScaleSnapValue{ 1.f };
		float mRotSnapValue{ 45.f };
	};

	class SceneWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		SceneWindow();
		void InitilializeFrameBuffer();
		void RunFrameBuffer();
		void OnKeyPressed();
	private:
		glm::vec2 mViewportSize;
		SnapValueSettings mSnapSettings;
		// Hold Frame Buffer Object
		std::shared_ptr<FrameBuffer> m_frameBuffer;
		// Hold Gizmo Type
		int m_GizmoType{ -1 };
	};
}
