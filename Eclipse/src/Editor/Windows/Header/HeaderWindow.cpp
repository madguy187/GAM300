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
		ECGui::InsertSameLine(ECGui::GetWindowSize().x / 2.3f);

		if (!engine->GetPlayState())
		{
			if (ECGui::ButtonBool("Play " ICON_FA_PLAY))
			{
				engine->mono.StartMono();
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
				engine->mono.StopMono();

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
	}

	void HeaderWindow::UtilitiesButtons()
	{
		ImGui::NewLine();

		ECGui::InsertSameLine(ECGui::GetWindowSize().x /3.0f);
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W)))
		{
			HeaderWindow::Translate();
		}
		if (ImGui::Button(ICON_FA_ARROWS_ALT, ImVec2{ 100,20 }))
		{

			HeaderWindow::Translate();

		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Select and translate objects (W)");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ECGui::InsertSameLine();
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_E)))
		{
			HeaderWindow::Rotate();
		}
		if (ImGui::Button(ICON_FA_SPINNER ICON_FA_REPLY, ImVec2{ 100,20 }))
		{

			HeaderWindow::Rotate();

		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Select and rotate objects (E)");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ECGui::InsertSameLine();
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R)))
		{
			HeaderWindow::Scale();
		}
		if (ImGui::Button(ICON_FA_EXPAND, ImVec2{ 100,20 }))
		{

			HeaderWindow::Scale();

		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Select and scale objects (R)");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ECGui::InsertSameLine();

		if (ImGui::Button(ICON_FA_VIDEO_CAMERA, ImVec2{ 100,20}))
		{
			ImGui::OpenPopup("Camera Setting");
		}
		if (ImGui::BeginPopup("Camera Setting"))
		{
			ImGui::SetScrollY(5);
			ChildSettings settings{ "Camera Setting",ImVec2{200,45} };
			ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::CameraSetting,this));
			ImGui::EndPopup();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Camera Settings");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ECGui::InsertSameLine();
		auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
		//scene->SetSnapping(true);
		static bool isActive;
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftControl)))
		{
			if (!isActive)
			{
				isActive = true;
				scene->SetSnapping(isActive);
				EDITOR_LOG_INFO("Snapping is turned off.")
			}
			else
			{
				isActive = false;
				scene->SetSnapping(isActive);
				EDITOR_LOG_INFO("Snapping is turned off.")
			}
		}

		if(scene->GetSnapping())
		{

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 0, 0)));
			if (ImGui::Button("Snap " ICON_FA_ODNOKLASSNIKI_SQUARE, ImVec2{ 100,20 }))
			{
				ImGui::OpenPopup("Snapping");
			}
			ImGui::PopStyleColor();

			if (ImGui::BeginPopup("Snapping"))
			{
				ChildSettings settings{ "Snapping",ImVec2{400,170} };
				ECGui::DrawChildWindow<void(bool&)>(settings, std::bind(&HeaderWindow::SnappingManager, this, std::placeholders::_1), isActive);

				ImGui::EndPopup();
			}
		}
		else
		{

			if (ImGui::Button("Snap " ICON_FA_ODNOKLASSNIKI_SQUARE, ImVec2{ 100,20 }))
			{
				ImGui::OpenPopup("Snapping");
			}

			if (ImGui::BeginPopup("Snapping"))
			{
				ChildSettings settings{ "Snapping",ImVec2{400,170} };
				ECGui::DrawChildWindow<void(bool&)>(settings, std::bind(&HeaderWindow::SnappingManager, this, std::placeholders::_1), isActive);

				ImGui::EndPopup();
			}

		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Enable or disables snapping to the grid when dragging objects around.\n Changes POS,ROT,SCALE of snap.");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		//ECGui::InsertSameLine();
		//int posSnapVal = static_cast<int>(scene->GetSnapSettings().mPosSnapValue);
		//if (ImGui::Button(std::to_string(posSnapVal).c_str(), ImVec2{ 50,20 }))
		//{
		//	ImGui::OpenPopup("Set Pos Snap");
		//}
		//if (ImGui::BeginPopup("Set Pos Snap"))
		//{
		//	ChildSettings settings{ "Set Pos Snap",ImVec2{200,45} };
		//	ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::SetPosSnap, this));
		//	ImGui::EndPopup();
		//}
		//if (ImGui::IsItemHovered())
		//{
		//	ImGui::BeginTooltip();
		//	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		//	ImGui::TextUnformatted("Set the Position Grid Snap Value.");
		//	ImGui::PopTextWrapPos();
		//	ImGui::EndTooltip();
		//}
		//
		//ECGui::InsertSameLine();
		//int RotSnapVal = static_cast<int>(scene->GetSnapSettings().mRotSnapValue);
		//std::string temp = std::to_string(RotSnapVal);
		//const char* degree = "*";
		//std::string c = temp + degree;
		//if (ImGui::Button(c.c_str(), ImVec2{50,20}))
		//{
		//	ImGui::OpenPopup("Set Rot Snap");
		//}
		//if (ImGui::BeginPopup("Set Rot Snap"))
		//{
		//	ChildSettings settings{ "Set Rot Snap",ImVec2{200,45} };
		//	ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::SetRotSnap, this));
		//	ImGui::EndPopup();
		//}
		//if (ImGui::IsItemHovered())
		//{
		//	ImGui::BeginTooltip();
		//	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		//	ImGui::TextUnformatted("Set the Rotation Grid Snap Value.");
		//	ImGui::PopTextWrapPos();
		//	ImGui::EndTooltip();
		//}
		//
		//ECGui::InsertSameLine();
		//int scaleSnapVal = static_cast<int>(scene->GetSnapSettings().mScaleSnapValue);
		//if (ImGui::Button(std::to_string(scaleSnapVal).c_str(), ImVec2{ 50,20 }))
		//{
		//	ImGui::OpenPopup("Set Scale Snap");
		//}
		//if (ImGui::BeginPopup("Set Scale Snap"))
		//{
		//	ChildSettings settings{ "Set Scale Snap",ImVec2{200,45} };
		//	ECGui::DrawChildWindow<void()>(settings, std::bind(&HeaderWindow::SetScaleSnap, this));
		//	ImGui::EndPopup();
		//}
		//if (ImGui::IsItemHovered())
		//{
		//	ImGui::BeginTooltip();
		//	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		//	ImGui::TextUnformatted("Set the Scale Grid Snap Value.");
		//	ImGui::PopTextWrapPos();
		//	ImGui::EndTooltip();
		//}
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

	void HeaderWindow::SnappingManager(bool& isActive)
	{
		auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();

		ECGui::CheckBoxBool("Enable Snapping", &isActive,false);
		scene->SetSnapping(isActive);

		ECGui::DrawTextWidget<const char*>("Pos Snap:", "");
		ECGui::DrawSliderFloatWidget("Pos Snap", &scene->GetRefToSnapSettings().mPosSnapValue, true, 1.f, 100.f);
		ECGui::DrawTextWidget<const char*>("Rot Snap:", "");
		ECGui::DrawSliderFloatWidget("Rot Snap", &scene->GetRefToSnapSettings().mRotSnapValue, true, 1.f, 100.f);
		ECGui::DrawTextWidget<const char*>("Scale Snap:", "");
		ECGui::DrawSliderFloatWidget("Scale Snap", &scene->GetRefToSnapSettings().mScaleSnapValue, true, 1.f, 100.f);
	}

	//void HeaderWindow::SetPosSnap()
	//{
	//	auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
	//
	//	ECGui::DrawTextWidget<const char*>("Pos Snap:", "");
	//
	//	ECGui::DrawSliderFloatWidget("Pos Snap", &scene->GetRefToSnapSettings().mPosSnapValue, true, 1.f, 100.f);
	//}
	//
	//void HeaderWindow::SetRotSnap()
	//{
	//	auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
	//
	//	ECGui::DrawTextWidget<const char*>("Rot Snap:", "");
	//
	//	ECGui::DrawSliderFloatWidget("Rot Snap", &scene->GetRefToSnapSettings().mRotSnapValue, true, 1.f, 100.f);
	//}
	//
	//void HeaderWindow::SetScaleSnap()
	//{
	//	auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
	//
	//	ECGui::DrawTextWidget<const char*>("Scale Snap:", "");
	//
	//	ECGui::DrawSliderFloatWidget("Scale Snap", &scene->GetRefToSnapSettings().mScaleSnapValue, true, 1.f, 100.f);
	//}

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