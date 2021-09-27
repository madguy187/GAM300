#include "pch.h"
#include "HeaderWindow.h"
#include "ECS/SystemManager/Systems/System/MonoSystem/MonoSystem.h"

namespace Eclipse
{
	void HeaderWindow::Update()
	{
		HideTabBar(IsTabBarHidden);

		if (ECGui::BeginMainWindowWithFlag(WindowName), NULL,
			/*ImGuiWindowFlags_NoTitleBar |*/ ImGuiWindowFlags_NoScrollbar)
		{
			RunPlayPauseStep();
			UtilitiesButtons();
		}

		ECGui::EndMainWindow();
	}

	void HeaderWindow::Init()
	{
		Type = EditorWindowType::EWT_HEADER;
		WindowName = "Header";
		IsVisible = true;
	}

	void HeaderWindow::Unload()
	{}

	void HeaderWindow::RunPlayPauseStep()
	{
		ECGui::InsertSameLine(ECGui::GetWindowSize().x / 2.f);

		if (!engine->GetPlayState())
		{
			if (ECGui::ButtonBool("Play " ICON_FA_PLAY))
			{
				engine->mono.RestartMono();
				auto& mono = engine->world.GetSystem<MonoSystem>();
				mono->Init();

				engine->SetPlayState(true);
				ImGui::SetWindowFocus("Game View");
				EDITOR_LOG_INFO("Scene is playing...");
			}
		}
		else
		{
			if (ECGui::ButtonBool("Stop " ICON_FA_STOP))
			{
				auto& mono = engine->world.GetSystem<MonoSystem>();
				mono->Terminate();
				engine->mono.ResetMono();

				engine->SetPlayState(false);
				engine->SetPauseState(false);
				ImGui::SetWindowFocus("Scene View");
				EDITOR_LOG_INFO("Scene has stopped playing. Reverting to original state...");
			}
		}

		ECGui::InsertSameLine();
		if (ECGui::ButtonBool("Pause " ICON_FA_PAUSE))
		{
			if (engine->GetPlayState())
			{
				if (engine->GetPauseState())
				{
					engine->SetPauseState(false);
					EDITOR_LOG_INFO("Scene is unpaused!");
				}
				else
				{
					engine->SetPauseState(true);
					EDITOR_LOG_INFO("Scene is paused!");
				}
			}
			else
			{
				EDITOR_LOG_WARN("Scene has to be playing first.");
			}
		}

		ECGui::InsertSameLine();
		if (ECGui::ButtonBool("Step " ICON_FA_STEP_FORWARD))
		{
			if (engine->GetPlayState())
			{
				engine->SetStepState(true);
				EDITOR_LOG_INFO("Stepped scene.");
			}
			else
			{
				EDITOR_LOG_WARN("Scene has to be playing first.");
			}
		}
		ImGui::NewLine();
	}

	void HeaderWindow::UtilitiesButtons()
	{

		ECGui::InsertSameLine(ECGui::GetWindowSize().x/1.25f);
		if (ImGui::Button(ICON_FA_ARROWS_ALT, ImVec2{ 50,20 }))
		{
			HeaderWindow::Translate();
			//ECGui::DrawInputTextHintWidget("Tranalsate","Select and Translate Objects(W)",100);
			EDITOR_LOG_INFO("Translate");
		}
		ECGui::InsertSameLine();

		if (ImGui::Button(ICON_FA_SPINNER ICON_FA_REPLY, ImVec2{ 50,20 }))
		{
			HeaderWindow::Rotate();
		}
		ECGui::InsertSameLine();

		if (ImGui::Button(ICON_FA_EXPAND, ImVec2{ 50,20 }))
		{
			HeaderWindow::Scale();
		}
		ECGui::InsertSameLine();

		if (ImGui::Button(ICON_FA_VIDEO_CAMERA, ImVec2{50,20}))
		{
			ImGui::OpenPopup("Camera Setting");
		}
		if (ImGui::BeginPopup("Camera Setting"))
		{
			ImGui::SetScrollY(5);
			ChildSettings settings{ "Camera Setting",ImVec2{100,100} };
			ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::CameraSetting,this));
			ImGui::EndPopup();
		}
	}

	void HeaderWindow::CameraSetting()
	{
		if (engine->gCamera.GetEditorCameraID() != MAX_ENTITY)
		{
			auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

			ECGui::DrawTextWidget<const char*>("Camera Speed:", "");		

			ECGui::DrawSliderFloatWidget("CamSpeed", &camera.cameraSpeed, true, 1.f, 200.f);

		}
	}

	void HeaderWindow::Translate()
	{
			Entity item = engine->editorManager->GetSelectedEntity();

			auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();

			if (engine->world.CheckComponent<TransformComponent>(item))
			{
				scene->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
			}

	}

	void HeaderWindow::Rotate()
	{
		Entity item = engine->editorManager->GetSelectedEntity();

		auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();

		if (engine->world.CheckComponent<TransformComponent>(item))
		{
			scene->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
		}
	}

	void HeaderWindow::Scale()
	{
		Entity item = engine->editorManager->GetSelectedEntity();

		auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();

		if (engine->world.CheckComponent<TransformComponent>(item))
		{
			scene->SetGizmoType(ImGuizmo::OPERATION::SCALE);
		}
	}

	void HeaderWindow::HideTabBar(bool active)
	{
		if (!active)
		{
			ImGuiWindowClass windowClass;
			windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
			ImGui::SetNextWindowClass(&windowClass);
			IsTabBarHidden = true;
		}
	}
}