#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class MeshEditorWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void RunMainWindow();
		void RunFrameBuffer();
		void RunMeshSettings();
		void OnCameraMoveEvent();
		void OnCameraZoomEvent();

		void SetMeshID(Entity ID);
		Entity GetMeshID();
		bool GetActiveState();
	private:
		FrameBuffer* m_frameBuffer{ nullptr };
		ECVec2 mViewportSize{ 0.f, 0.f };
		Entity MeshID{ MAX_ENTITY };
		bool IsActive{ false };
	};
}
