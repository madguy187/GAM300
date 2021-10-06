#include "pch.h"
#include "HeaderWindow.h"
#include "ECS/SystemManager/Systems/System/MonoSystem/MonoSystem.h"

namespace Eclipse
{
	void HeaderWindow::Update()
	{
		HideTabBar(IsTabBarHidden);

		if (ECGui::BeginMainWindowWithFlag(WindowName), NULL,
			ImGuiWindowFlags_NoScrollbar)
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
		ECGui::InsertSameLine(ECGui::GetWindowSize().x / 2.3f);

		if (!engine->GetPlayState())
		{
			if (ECGui::ButtonBool("Play " ICON_FA_PLAY))
			{
				engine->szManager.SaveBackupFile();
				engine->mono.StartMono();
				std::shared_ptr<MonoSystem>& mono = engine->world.GetSystem<MonoSystem>();
				mono->Init();

				engine->SetPlayState(true);
				ECGui::SetWindowFocus("Game View");
				EDITOR_LOG_INFO("Scene is playing...");
			}
		}
		else
		{
			if (ECGui::ButtonBool("Stop " ICON_FA_STOP))
			{
				std::shared_ptr<MonoSystem>& mono = engine->world.GetSystem<MonoSystem>();
				mono->Terminate();
				engine->mono.StopMono();

				engine->SetPlayState(false);
				engine->SetPauseState(false);
				ECGui::SetWindowFocus("Scene View");
				engine->szManager.LoadBackupFile();
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
	}

	void HeaderWindow::UtilitiesButtons()
	{
		ECGui::NewLine();
		ECGui::InsertSameLine(ECGui::GetWindowSize().x / 3.0f);

		if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_W)))
		{
			HeaderWindow::Translate();
		}
		if (ECGui::ButtonBool(ICON_FA_ARROWS_ALT, ImVec2{ 100,20 }))
		{

			HeaderWindow::Translate();

		}
		if (ECGui::IsItemHovered())
		{
			ECGui::BeginToolTip();
			ECGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ECGui::TextUnformatted("Select and translate objects (W)");
			ECGui::PopTextWrapPos();
			ECGui::EndTooltip();
		}

		ECGui::InsertSameLine();
		if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_E)))
		{
			HeaderWindow::Rotate();
		}
		if (ECGui::ButtonBool(ICON_FA_SPINNER ICON_FA_REPLY, ImVec2{ 100,20 }))
		{

			HeaderWindow::Rotate();

		}
		if (ECGui::IsItemHovered())
		{
			ECGui::BeginToolTip();
			ECGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ECGui::TextUnformatted("Select and rotate objects (E)");
			ECGui::PopTextWrapPos();
			ECGui::EndTooltip();
		}

		ECGui::InsertSameLine();

		if (ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_R)))
		{
			HeaderWindow::Scale();
		}

		if (ECGui::ButtonBool(ICON_FA_EXPAND, ImVec2{ 100,20 }))
		{

			HeaderWindow::Scale();

		}

		if (ECGui::IsItemHovered())
		{
			ECGui::BeginToolTip();
			ECGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ECGui::TextUnformatted("Select and scale objects (R)");
			ECGui::PopTextWrapPos();
			ECGui::EndTooltip();
		}

		ECGui::InsertSameLine();

		if (ECGui::ButtonBool(ICON_FA_VIDEO_CAMERA, ImVec2{ 100,20}))
		{
			ECGui::OpenPopup("Camera Setting");
		}

		if (ImGui::BeginPopup("Camera Setting"))
		{
			ECGui::SetScrollY(5);
			ChildSettings settings{ "Camera Setting",ImVec2{200,45} };
			ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::CameraSetting,this));
			ECGui::EndPopup();
		}

		if (ECGui::IsItemHovered())
		{
			ECGui::BeginToolTip();
			ECGui::PushTextWrapPos(ECGui::GetFontSize() * 35.0f);
			ECGui::TextUnformatted("Camera Settings");
			ECGui::PopTextWrapPos();
			ECGui::EndTooltip();
		}

		ECGui::InsertSameLine();
		auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
		static bool isActive = false;

		if(scene->GetSnapping())
		{
			ECGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 0, 0)));

			if (ECGui::ButtonBool("Snap " ICON_FA_ODNOKLASSNIKI_SQUARE, ImVec2{ 100,20 }))
			{
				ECGui::OpenPopup("Snapping");
			}

			ECGui::PopStyleColor();

			if (ECGui::BeginPopup("Snapping"))
			{
				ChildSettings settings{ "Snapping",ImVec2{400,170} };
				ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::SnappingManager, this));
				ECGui::EndPopup();
			}
		}
		else
		{
			if (ECGui::ButtonBool("Snap " ICON_FA_ODNOKLASSNIKI_SQUARE, ImVec2{ 100,20 }))
			{
				ECGui::OpenPopup("Snapping");
			}

			if (ECGui::BeginPopup("Snapping"))
			{
				ChildSettings settings{ "Snapping",ImVec2{400,170} };
				ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::SnappingManager, this));
				ECGui::EndPopup();
			}
		}

		if (ECGui::IsItemHovered())
		{
			ECGui::BeginToolTip();
			ECGui::PushTextWrapPos(ECGui::GetFontSize() * 35.0f);
			ECGui::TextUnformatted("Enable or disables snapping to the grid when dragging objects around.\n Changes POS,ROT,SCALE of snap.");
			ECGui::PopTextWrapPos();
			ECGui::EndTooltip();
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

	void HeaderWindow::SnappingManager()
	{
		auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
		static bool IsActive = false;
		static size_t posValueIndex = 0;
		static size_t rotValueIndex = 0;
		static size_t scaleValueIndex = 0;
		if (ECGui::CheckBoxBool("Enable Snapping", &IsActive, false))
		{
			scene->SetSnapping(IsActive);
		}

		ECGui::DrawTextWidget<const char*>("Pos Snap:", "");
		std::vector<std::string> posValues = { "1","5","10","50","100","500","1000","5000","10000" };
		ECGui::CreateComboList({"Pos Snap:"}, posValues, posValueIndex);
		scene->GetRefToSnapSettings().mPosSnapValue = static_cast<float>(std::stoi(posValues[posValueIndex]));

		//ECGui::DrawSliderFloatWidget("Pos Snap", &scene->GetRefToSnapSettings().mPosSnapValue, true, 1.f, 100.f);
		ECGui::DrawTextWidget<const char*>("Rot Snap:", "");
		std::vector<std::string> rotValues = { "5","10","15","30","45","60","90","120"};
		ECGui::CreateComboList({ "Rot Snap:" }, rotValues, rotValueIndex);
		scene->GetRefToSnapSettings().mRotSnapValue = static_cast<float>(std::stoi(rotValues[rotValueIndex]));

		//ECGui::DrawSliderFloatWidget("Rot Snap", &scene->GetRefToSnapSettings().mRotSnapValue, true, 1.f, 100.f);
		ECGui::DrawTextWidget<const char*>("Scale Snap:", "");
		std::vector<std::string> scaleValues = { "10","1","0.5","0.25","0.125","0.0625","0.03125"};
		ECGui::CreateComboList({ "Scale Snap:" }, scaleValues, scaleValueIndex);
		scene->GetRefToSnapSettings().mScaleSnapValue = std::stof(scaleValues[scaleValueIndex]);
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