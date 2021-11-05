#pragma once

#include "../Interface/ECGuiWindow.h"
#include "../NodeEditor/NodeEditor.h"
namespace Eclipse
{
	class MaterialEditorWindow final : public ECGuiWindow
	{
		MaterialNode MaterialEditor;

	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void RunMainWindow();
		void RunFrameBuffer();
		void OnCameraZoomEvent();
		void OnCameraMoveEvent();
		void RunMaterialSettings();
		void CheckCurrentMaterial(size_t comboIndex);

		ECVec3 ColorPicker{ 1.0f };
	private:
		FrameBuffer* m_frameBuffer{ nullptr };
		ECVec2 mViewportSize{ 0.f, 0.f };
		size_t comboindex = 0;

		void Buttons();
		bool ShowTransformProperty(const char* name, ImGuiTextFilter& filter);
		bool ShowMaterialProperty(const char* name, ImGuiTextFilter& filter);
	};
}