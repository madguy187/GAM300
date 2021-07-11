#include "pch.h"
#include "Debug.h"
#include "../Scene/Scene.h"

namespace Eclipse
{
	void DebugWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&DebugWindow::DrawImpl, this));
	}

	DebugWindow::DebugWindow()
	{
		Type = EditorWindowType::EWT_DEBUG;
		WindowName = "Debug";
	}

	void DebugWindow::DrawImpl()
	{
		auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
		ECGui::PushItemWidth(ECGui::GetWindowSize().x / 3.5f);

		ECGui::DrawTextWidget<const char*>("Snap Settings:", "");
		{
			ECGui::DrawTextWidget<const char*>("Pos", "");
			ECGui::InsertSameLine();
			ECGui::DrawInputFloatWidget("PosSnap", &scene->GetSnapSettings().mPosSnapValue, true, 0.5f);
			ECGui::InsertSameLine();

			ECGui::DrawTextWidget<const char*>("Scale", "");
			ECGui::InsertSameLine();
			ECGui::DrawInputFloatWidget("ScaleSnap", &scene->GetSnapSettings().mScaleSnapValue, true, 0.5f);
			ECGui::InsertSameLine();

			ECGui::DrawTextWidget<const char*>("Rot", "");
			ECGui::InsertSameLine();
			ECGui::DrawInputFloatWidget("RotSnap", &scene->GetSnapSettings().mRotSnapValue, true, 45.f);
		}

		if (engine->gCamera.GetEditorCameraID() != MAX_ENTITY)
		{
			auto& camCom = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

			ECGui::DrawTextWidget<const char*>("Camera Speed:", "");
			{
				ECGui::DrawTextWidget<const char*>("Speed", "");
				ECGui::InsertSameLine();
				ECGui::DrawSliderFloatWidget("CamSpeed", &camCom.cameraSpeed, true, 1.f, 30.f);
			}
		}
	}
}
