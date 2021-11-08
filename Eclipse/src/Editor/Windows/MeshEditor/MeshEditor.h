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
		Entity GetMeshID() const;
		Entity GetOldestParentID() const;
		bool GetActiveState();
	private:
		FrameBuffer* m_frameBuffer{ nullptr };
		ECVec2 mViewportSize{ 0.f, 0.f };
		Entity MeshID{ MAX_ENTITY };
		Entity OldestParentID{ MAX_ENTITY };
		std::vector<Entity> MeshFamily;
		int MeshIndex{ 0 };
		bool IsActive{ false };

		//Temporary, can move to any where else.
		void RecursiveDestroy(const Entity& ent);
	};
}
