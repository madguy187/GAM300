/**********************************************************************************
* \file            UiDatas.h
*
* \brief        Definition of the UiData class function(s).
*
* \author        Tian Yu
*
* \email		t.yu\@digipen.edu
*
* \date			9 oct 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef UI_DATAS_H
#define UI_DATAS_H
#include <imgui.h>
#include <string>
class World;
class UiData
{
public:
	static void Gui_Datas(World& world, bool& scroll);
	static void Gui_Objects(bool* show_app_debug_log, World& world);
	static void Gui_Data_Info_Debug(World& world);
};

#endif//UI_DATAS_H