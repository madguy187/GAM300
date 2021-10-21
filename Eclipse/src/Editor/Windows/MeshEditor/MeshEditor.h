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
	private:
		FrameBuffer* m_frameBuffer{ nullptr };
		ECVec2 mViewportSize{ 0.f, 0.f };
	};
}