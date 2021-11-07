#pragma once
#include "pch.h"
#include "mono/jit/jit.h"
// #include "Graphics/InputHandler/AllInputKeyCodes.h" Nico , if u ever need to reference my key codes

namespace Eclipse
{
	static void GetKeyTriggered(InputKeycode KeyIn)
	{
		engine->InputManager->GetKeyTriggered(KeyIn);
	}

	static void GetKeyTriggered(MonoString* Mappedkeycode)
	{
		engine->InputManager->GetKeyTriggered(mono_string_to_utf8(Mappedkeycode));
	}

	static bool GetKeyCurrentByKeyCode(uint32_t keycode)
	{
		return engine->InputManager->GetKeyCurrent(static_cast<InputKeycode>(keycode));
	}

	static bool GetKeyCurrentByName(MonoString* Mappedkeycode)
	{
		return engine->InputManager->GetKeyCurrent(mono_string_to_utf8(Mappedkeycode));
	}

	// I haven do UI FOR MOUSE YET
	static void GetMouseTriggered(InputMouseKeycode KeyIn)
	{
		engine->InputManager->GetMouseTriggered(KeyIn);
	}

	static void GetMouseTriggered(std::string Mappedkeycode)
	{
		engine->InputManager->GetMouseTriggered(Mappedkeycode);
	}

	static void GetMouseCurrent(InputMouseKeycode keycode)
	{
		engine->InputManager->GetMouseCurrent(keycode);
	}

	static void GetMouseCurrent(std::string Mappedkeycode)
	{
		engine->InputManager->GetMouseCurrent(Mappedkeycode);
	}

	static float GetMouseAxis(MonoString* Axis)
	{
		std::string axis = mono_string_to_utf8(Axis);
		return engine->InputManager->GetAxis(axis);
	}
}