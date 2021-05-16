/**********************************************************************************
* \file            ImGui_Gam200.cpp
*
* \brief        Definition of the ImGui_Gam200 class function(s).
*
* \author        Tian Yu
*
* \email		t.yu\@digipen.edu
*
* \date			 1 sep 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ImGui_Gam200.h"
#include <array>
#include "OpenFileDialog.h"
#include "GameStateManager.h"
#include <World.h>
#include "DataSystemPerformance.h"
#include "Editor.h"
#include "EditorPause.h"

vec3 scale3(0, 0,0);
vec3 translate3(0, 0, 0);
static bool show_app_style_editor = false;
static bool show_app_main_menu_bar = false;
static bool show_app_debug_log = false;
static bool show_app_debug_overlay = false;
static bool show_app_metricswindow = false;
static bool dockChecker = false;
extern bool pause;
extern bool gamePause;
/// Declaring function
void ShowTopDisplayMenuBar(World& world);
void ShowMenuFile(World& world);
void UserGuide();
void Controls();
void ImGui_LayOut::Gui_OnDocking(bool* checker)
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	
	ImGui::Begin("DockSpace", checker, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Dock Options"))
		{
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();
			
			if (ImGui::MenuItem("Close", NULL, false, checker != NULL))
				*checker = false;
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void ImGui_LayOut::Gui_StyleEditor(bool*_show_app_style_editor)
{
	if (_show_app_style_editor)
	{
		ImGui::Begin("ImGui Style Editor", _show_app_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
}

void ImGui_LayOut::Gui_Menu(bool* checker, World& world)
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");
	static bool show_app_metrics = false;
	static bool _show_app_style_editor = false;
	static bool show_app_about = false;

	if (show_app_metrics) { ImGui::ShowMetricsWindow(&show_app_metrics); }
	if (show_app_about) { ImGui::ShowAboutWindow(&show_app_about); }
	if (_show_app_style_editor)
	{
		ImGui::Begin("Dear ImGui Style Editor", &_show_app_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool no_docking = false;
	static bool system_performance = false;
	if (show_app_main_menu_bar)
	{
		ShowTopDisplayMenuBar(world);
	}
	if (show_app_style_editor)
	{
		Gui_StyleEditor(&show_app_style_editor);
	}
	if (show_app_metricswindow)
	{
		ImGui::ShowMetricsWindow(&show_app_metricswindow);
	}
	Gui_OnDocking(&dockChecker);
	
	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (no_docking)         window_flags |= ImGuiWindowFlags_NoDocking;
	if (no_close)           checker = NULL; // Don't pass our bool* to Begin

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x + 650, main_viewport->GetWorkPos().y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("All Functions here", checker, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	// Menu Bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile(world);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("ShortCut"))
		{
			ImGui::MenuItem("(Enable menu bar)", NULL, false, false);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("(Edit Ui)", NULL, false, false);
			ImGui::MenuItem("Ui Editor", NULL, &show_app_style_editor);
			ImGui::MenuItem("Debug Overlay", NULL, &show_app_debug_overlay);
			ImGui::MenuItem("Metrics Window", NULL, &show_app_metricswindow);
			ImGui::MenuItem("Docking Window", NULL, &dockChecker);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Checkbox("Toggle Game StateManager", &(GSM::GetActive()));
	ImGui::Checkbox("Enable System Performance", &system_performance);
	if(system_performance)
	{
		DataSystemPerformance::SystemTimeTotal();
	}
	
	if (ImGui::CollapsingHeader("USER GUIDE:"))
	{
		UserGuide();
	}
	if (ImGui::CollapsingHeader("CONTROLS:"))
	{
		Controls();
	}
	ImGui::Begin("Objects ");
	UiData::Gui_Objects(&show_app_debug_log, world);
	ImGui::End();
	EditorPause::Pause(pause, world);
	ImGui::End();
}

void ImGui_LayOut::Gui_InsertSeparator(int count)
{
	for (int i = 0; i <= count; ++i)
	{
		ImGui::Separator();
	}
}

void ImGui_LayOut::HelpMarker(const char* input)
{
	ImGui::PushStyleColor(ImGuiCol_TextDisabled,(ImVec4)ImColor::HSV(1, 1.f, 1.6f,1.f));
	ImGui::TextDisabled("(?)");
	ImGui::PopStyleColor(1);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(input);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
void ShowMenuFile(World& world)
{
	ImGui::MenuItem("(File options)", NULL, false, false);
	if (ImGui::MenuItem("New", "Ctrl+N"))
	{
		/*do something create new file here*/
		FileDialog::SaveFile();
		GSM::SetNextState(GameStates::RESTART);
	}
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		FileDialog::FileBrowser();
		GSM::SetNextState(GameStates::RESTART);
	}
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		//save on current scene
		if (FileDictionary::GetSceneFile().empty())
		{
			FileDialog::SaveFile();
			world.Serialize();
		}
		else
		{
			FileDictionary::SetDirFromScene();
			world.Serialize();
		}
	}
	if (ImGui::MenuItem("Save As.."))
	{
		FileDialog::SaveFile();
		world.Serialize();
	}
	if (ImGui::MenuItem("Quit", "Alt+F4"))
	{
		GSM::SetNextState(GameStates::EXIT);
	}
}

void ShowTopDisplayMenuBar(World& world)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile(world);
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Not working")) {/*undo function*/ }
			if (ImGui::MenuItem("Redo", "Not working", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Not working")) {/*cut funciton*/ }
			if (ImGui::MenuItem("Copy", "Not working")) {/*copy funciton*/ }
			if (ImGui::MenuItem("Paste", "Not working")) {/*paste funciton*/ }
			ImGui::EndMenu();
		}
		ImGui::Separator();
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("UI Style Editor", NULL, &show_app_style_editor);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void UserGuide()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::BulletText("Double-click on title bar to collapse window.");
	ImGui::BulletText(
		"Click and drag on lower corner to resize window\n"
		"(double-click to auto fit window to its contents).");
	if (io.FontAllowUserScaling)
		ImGui::BulletText("CTRL+Mouse Wheel to zoom window contents.");
	ImGui::BulletText("While inputing text:\n");
	ImGui::Indent();
	ImGui::BulletText("CTRL+X/C/V to use clipboard cut/copy/paste.(not added yet)");
	ImGui::BulletText("CTRL+Z,CTRL+Y to undo/redo.(not added yet)");
	ImGui::Unindent();
}

void Controls()
{
	ImGui::BulletText("Press H to decrease angular displacement");
	ImGui::BulletText("Press K to decrease angular displacement");
	ImGui::BulletText("Peess U to displacement camera in the direction of its up vector");
	ImGui::BulletText("Press V to toggle between free and first-person camera implementations");
	ImGui::BulletText("Press Z to zoom in and out from the scene");
}

ImGui_LayOut::AppLog::AppLog()
{
	AutoScroll = false;
	Clear();
}

void ImGui_LayOut::AppLog::Clear()
{
	Buf.clear();
	LineOffsets.clear();
	LineOffsets.push_back(0);
}

void ImGui_LayOut::AppLog::AddLog(const char* fmt, ...)
{
	int old_size = Buf.size();
	va_list args;
	va_start(args, fmt);
	Buf.appendfv(fmt, args);
	va_end(args);
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
		if (Buf[old_size] == '\n')
			LineOffsets.push_back(old_size + 1);
}

void ImGui_LayOut::AppLog::Draw(const char* title, bool* p_open)
{
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}
	
	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::EndPopup();
	}

	// Main window
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear)
		Clear();
	if (copy)
		ImGui::LogToClipboard();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const char* buf = Buf.begin();
	const char* buf_end = Buf.end();
	if (Filter.IsActive())
	{
		for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
		{
			const char* line_start = buf + LineOffsets[line_no];
			const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
			if (Filter.PassFilter(line_start, line_end))
				ImGui::TextUnformatted(line_start, line_end);
		}
	}
	else
	{
		ImGuiListClipper clipper;
		clipper.Begin(LineOffsets.Size);
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();
	}
	ImGui::PopStyleVar();

	if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();
}


