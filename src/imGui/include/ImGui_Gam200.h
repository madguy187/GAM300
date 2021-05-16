/**********************************************************************************
* \file            ImGui_Gam200.h
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
#ifndef IMGUI_GAM200_H
#define IMGUI_GAM200_H

#include <ctime>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "GLHelper.h"
#include "Graphics.h"
#include "AllComponents.h"
#include "UiDatas.h"
class World;
class ImGui_LayOut 
{
public:
	struct AppLog
	{

		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool                AutoScroll = false;  // Keep scrolling if already at the bottom.
		AppLog();
		void    Clear();
		void    AddLog(const char* fmt, ...) IM_FMTARGS(2);
		void    Draw(const char* title, bool* p_open = NULL);
	};

	void Gui_OnDocking(bool* dockChecker);

	void Gui_StyleEditor(bool* show_app_style_editor);

	void Gui_Menu(bool* checker,World & world);

	static void Gui_InsertSeparator(int count);

	static void HelpMarker(const char* input);
};















#endif// IMGUI_GAM200_H
